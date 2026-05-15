*This project has been created as part of the 42 curriculum by nirugger.*

---

# Codexion — Master the race for resources before the deadline masters you

## Description

Codexion is a concurrency simulation written in C. It models a co-working scenario where multiple **coders** (threads) sit around a circular table and take turns **compiling**, **debugging**, and **refactoring**. To compile, each coder must simultaneously hold two adjacent **USB dongles** — one on their left, one on their right — creating a classic resource-sharing problem analogous to the Dining Philosophers.

The simulation ends when either:
- every coder has compiled at least `number_of_compiles_required` times (success), or
- a coder fails to start compiling within `time_to_burnout` milliseconds since their last compile (burnout).

Key challenges addressed: deadlock avoidance, starvation prevention, fair scheduling (FIFO and EDF), dongle cooldown enforcement, precise burnout detection within 10 ms, and fully serialized logging.

---

## Instructions

### Compilation

```bash
make
```

This produces the `codexion` binary. The project compiles with `-Wall -Wextra -Werror -pthread`.

To remove object files:
```bash
make clean
```

To remove everything including the binary:
```bash
make fclean
```

To recompile from scratch:
```bash
make re
```

### Execution

```
./codexion number_of_coders time_to_burnout time_to_compile time_to_debug \
           time_to_refactor number_of_compiles_required dongle_cooldown scheduler
```

All arguments are mandatory. The scheduler must be either `fifo` or `edf`.

### Examples

5 coders, 800 ms burnout, 200 ms compile, 200 ms debug, 200 ms refactor, 10 compiles required, 100 ms cooldown, FIFO scheduling:
```bash
./codexion 5 800 200 200 200 10 100 fifo
```

Same scenario with Earliest Deadline First scheduling:
```bash
./codexion 5 800 200 200 200 10 100 edf
```

Single coder (only one dongle needed):
```bash
./codexion 1 500 100 100 100 5 50 fifo
```

### Output format

Every state change is logged as:
```
timestamp_in_ms X has taken a dongle
timestamp_in_ms X is compiling
timestamp_in_ms X is debugging
timestamp_in_ms X is refactoring
timestamp_in_ms X burned out
```

Where `timestamp_in_ms` is the elapsed time since the start of the simulation and `X` is the coder's ID (1-indexed).

---

## Blocking cases handled

### Deadlock prevention — Coffman's conditions

The classic deadlock scenario arises when every coder holds one dongle and waits for the other. This is avoided by enforcing a **global dongle acquisition order**: each coder always picks up the lower-indexed dongle first (`d_min`) and then the higher-indexed one (`d_max`). This is set up in `assign_dongles()` inside `codexion_init.c` and mirrors Dijkstra's resource-ordering solution. Breaking the circular wait condition (one of Coffman's four necessary conditions) is sufficient to guarantee the absence of deadlock.

### Starvation prevention

Under pure FIFO scheduling, a coder can be starved if a neighbour repeatedly acquires the shared dongle first. The EDF scheduler (`scheduler = "edf"`) addresses this by always granting the dongle to the coder whose burnout deadline (`last_compile_start + time_to_burnout`) is earliest, ensuring that the most urgent coder gets priority. Under both policies, the per-dongle wait queue (`t_queue`) in `queue.c` ensures that at most two coders ever contend for the same dongle, and `is_first()` enforces strict ordering so no coder can be bypassed indefinitely.

### Dongle cooldown

After a coder releases a dongle, the dongle becomes unavailable for `dongle_cooldown` milliseconds. This is enforced inside `take_dongle()` in `coder_routine.c`: the waiting loop checks both `d->is_free` and `get_time() - d->release_time < d->args->dongle_cooldown` before proceeding. `pthread_cond_timedwait` is used so the thread wakes at the exact moment the cooldown expires, avoiding busy-waiting.

### Precise burnout detection

A dedicated **monitor thread** (`monitor_routine` in `monitor.c`) polls all coders every 10 µs. For each coder it reads `c->burning` (the timestamp of the last compile start) under `c_mutex` and compares the elapsed time against `time_to_burnout`. When a coder exceeds the deadline, the monitor sets the global `sim->burnout` flag (under `burn_mutex`), prints the burnout message, and returns. The burnout message is guaranteed to appear within 10 ms of the actual deadline.

### Log serialization

All `printf` calls that produce coder-state output are wrapped inside `log_mutex`. This ensures that no two messages ever interleave across threads. The `log_msg()` helper in `coder_routine.c` acquires the log mutex, re-checks the burnout flag (to suppress stale messages printed after burnout is detected), prints the line, and releases the mutex atomically.

---

## Thread synchronization mechanisms

### Primitives used

| Primitive | Variable | Purpose |
|---|---|---|
| `pthread_mutex_t` | `dongle_mutex` (per dongle) | Protects `is_free`, `release_time`, and `queue[]` fields of each dongle |
| `pthread_cond_t` | `dongle_cond` (per dongle) | Allows coder threads to sleep while waiting for a dongle to become available and cooldown-free |
| `pthread_mutex_t` | `c_mutex` (per coder) | Protects `n_comp` and `burning` from concurrent reads by coder and monitor threads |
| `pthread_mutex_t` | `log_mutex` (global) | Serializes all stdout output so lines never interleave |
| `pthread_mutex_t` | `burn_mutex` (global) | Protects the global `burnout` flag shared between coder threads and the monitor |

### Dongle acquisition and the wait loop

When a coder needs a dongle it calls `take_dongle()`:

1. It acquires `dongle_mutex`.
2. It registers itself in `d->queue[]` via `update_queue_values()` (recording the current time and the coder's burnout deadline).
3. It enters a `while` loop that blocks using `pthread_cond_timedwait` until all three conditions hold simultaneously:
   - `d->is_free == TRUE`
   - the cooldown has elapsed (`get_time() - d->release_time >= dongle_cooldown`)
   - `is_first(d, c) == TRUE` (the coder is first in queue according to the active scheduler)
4. On waking, it checks the burnout flag before proceeding; if burnout has occurred it deregisters and exits cleanly.
5. On success, it sets `d->is_free = FALSE`, deregisters from the queue, and releases the mutex.

The `pthread_cond_timedwait` timeout is computed as `release_time + dongle_cooldown`, so the thread wakes at the exact cooldown expiry with no extra polling.

### Preventing race conditions on `n_comp` and `burning`

The coder thread writes to `c->n_comp` and `c->burning` after each compile. The monitor thread reads both of these fields periodically. All accesses on both sides go through `c_mutex`, ensuring no torn reads or writes.

### Thread-safe burnout signalling

When the monitor detects burnout it:
1. Locks `burn_mutex` and sets `sim->burnout = 1`.
2. Locks `log_mutex` and prints the burnout message.

Coder threads check `check_burnout()` at every scheduling point (inside `take_dongle()` and `log_msg()`). `check_burnout()` locks `burn_mutex` only for the duration of the flag read, keeping contention minimal. This design means coder threads observe the burnout flag within one scheduling quantum and exit their loops cleanly without needing explicit cancellation.

### Monitor thread lifecycle

The monitor runs until either all coders have finished the required number of compiles (`coders_out == number_of_coders`) or burnout is detected (`monitor_checks` returns `-1`). Coder threads are joined by `join_threads()` in `codexion.c` only after the monitor has returned, guaranteeing that no thread outlives the simulation.

---

## Resources

### References

- POSIX Threads Programming — Lawrence Livermore National Laboratory: https://hpc-tutorials.llnl.gov/posix/
- Dijkstra, E.W. — *Solution of a problem in concurrent programming control* (1965): the original resource-ordering deadlock solution
- Liu, C.L. & Layland, J.W. — *Scheduling algorithms for multiprogramming in a hard real-time environment* (1973): foundation of EDF scheduling
- The Linux `pthread_cond_timedwait(3)` man page: https://man7.org/linux/man-pages/man3/pthread_cond_timedwait.3p.html
- Coffman, E.G. et al. — *System deadlocks* (1971): the four necessary conditions for deadlock

### AI usage

Claude (Anthropic) was used as a support tool during this project in the following ways:

- **Design discussion**: brainstorming the queue structure for FIFO/EDF arbitration and validating that the resource-ordering approach correctly breaks the circular-wait condition.
- **Code review**: reviewing `take_dongle()` and `is_first()` for correctness of the timedwait timeout calculation and queue index mapping.
- **Documentation**: drafting and refining this README, in particular the technical descriptions of the synchronization mechanisms and blocking cases.

All code was written, reviewed, and fully understood by the author. AI-generated suggestions were verified against the subject requirements and tested before integration.