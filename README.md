*This project has been created as part of the 42 curriculum by nirugger.*

# Codexion

## Description

Codexion is a concurrency simulation inspired by the Dining Philosophers problem. A set of coders sit in a circular arrangement, each needing two adjacent USB dongles to compile quantum code. They cycle through three phases — **compile**, **debug**, **refactor** — and must never burn out (i.e., go too long without compiling).

The simulation ends either when every coder has compiled the required number of times, or when one burns out. The program uses POSIX threads, mutexes, and condition variables to coordinate access to shared resources while preventing deadlocks, starvation, and data races.

## Instructions

**Compilation:**
```bash
make
```

**Execution:**
```bash
./codexion <nb_coders> <time_to_burnout> <time_to_compile> <time_to_debug> \
           <time_to_refactor> <nb_compiles_required> <dongle_cooldown> \
           <scheduler> [--visual]
```

| Argument | Description |
|---|---|
| `nb_coders` | Number of coders (= number of dongles) |
| `time_to_burnout` | Max ms between compile starts before burnout |
| `time_to_compile` | Duration of the compile phase (ms) |
| `time_to_debug` | Duration of the debug phase (ms) |
| `time_to_refactor` | Duration of the refactor phase (ms) |
| `nb_compiles_required` | Compilations needed per coder to end the sim |
| `dongle_cooldown` | ms a dongle remains unavailable after release |
| `scheduler` | `fifo` (arrival order) or `edf` (earliest deadline first) |
| `--visual` | Optional: enables colored output |

**Example runs:**
```bash
make               # show Makefile help window
make run           # 5 coders, standard settings
make run--visual   # same, with colors
make val           # valgrind leak check
make hel           # helgrind race condition check
```

Return value: `0` if all coders compiled successfully, `1` if a burnout occurred or on error.

## Blocking Cases Handled

**Deadlock prevention** — Coffman's four conditions are broken by imposing a global dongle acquisition order: each coder always locks `d_min` (lower id) before `d_max` (higher id). This resource ordering eliminates circular wait entirely (`assign_dongles` in `codexion_init.c`).

**Starvation prevention** — The `scheduler` parameter enforces fair arbitration. With `fifo`, requests are served strictly in arrival order. With `edf`, the coder whose burnout deadline is soonest gets priority, preventing any coder from being indefinitely postponed when parameters are feasible.

**Cooldown handling** — After release, a dongle's `release_time` is recorded. Inside `take_dongle`, the coder re-checks `get_time() - d->release_time < dongle_cooldown` in a `pthread_cond_timedwait` loop, waking only when the cooldown has elapsed.

**Precise burnout detection** — A dedicated monitor thread polls every 10 µs. It compares `get_time() - coder->burning` against `time_to_burnout` under `c_mutex`. If the threshold is exceeded the global `burnout` flag is set atomically under `burn_mutex` and the message is printed immediately, guaranteeing the ≤10 ms precision requirement.

**Log serialization** — All `printf` calls are wrapped in a single `log_mutex` critical section. The lock is acquired, the burnout flag is checked (so no message leaks after burnout), the line is printed, and the lock is released — ensuring no two lines ever interleave.

## Thread Synchronization Mechanisms

**`pthread_mutex_t` — four distinct roles:**

- `dongle_mutex` (one per dongle): guards `is_free`, `release_time`, and the `queue[]` array. Any read or write to dongle state happens exclusively inside this lock.
- `c_mutex` (one per coder): protects `n_comp` and `burning` (the compile counter and last-compile timestamp). Both the coder thread and the monitor thread access these fields, so every access is bracketed by lock/unlock.
- `burn_mutex` (global): protects the shared `burnout` flag. Both `check_burnout` (called by coder threads) and `has_burned_out` (called by the monitor) acquire this mutex, preventing torn reads.
- `log_mutex` (global): serializes all output so that concurrent `printf` calls never produce interleaved lines.

**`pthread_cond_t` — one per dongle (`dongle_cond`):**

`take_dongle` enters a `pthread_cond_timedwait` loop under `dongle_mutex`. The timeout is set to the exact millisecond when the cooldown expires, so the thread sleeps efficiently rather than busy-waiting. When a coder releases a dongle it calls `pthread_cond_broadcast` (commented stub — broadcast is available but not used in the current version; the timed wait wakes naturally on expiry and re-evaluates all conditions atomically under the lock).

**Queue / scheduler (`queue.c`):**

Each dongle holds a two-slot `queue[2]` array (one slot per possible waiter, since only two coders can ever contend for the same dongle). `update_queue_values` stamps `request_time` (for FIFO) and `burnout_time` (for EDF) atomically inside `dongle_mutex`. `is_first` reads these fields — also inside the same lock — and returns whether the calling coder has priority, preventing any TOCTOU race on the queue state.

**Monitor–coder communication:**

The monitor never writes to coder state; it only reads `n_comp` and `burning` under `c_mutex`. The burnout signal flows through `burn_mutex` + the shared `burnout` integer. Coder threads poll `check_burnout()` at every scheduling point (before printing, inside `take_dongle`) so they self-terminate promptly without needing an explicit signal from the monitor.

## Resources

- My dearest friends and peers, aflorea, fbertozz, tvanni, who unraveled the misteries of multithreading in front of my very own eyes.
- also:
- [CodeVault playlist on unix threads in C](https://www.youtube.com/@CodeVault/playlists)
- [Dining Philosophers — original problem by E.W. Dijkstra](https://en.wikipedia.org/wiki/Dining_philosophers_problem)
- [pthread_cond_timedwait(3) man page](https://man7.org/linux/man-pages/man3/pthread_cond_timedwait.3p.html)
- [Helgrind: a thread error detector](https://valgrind.org/docs/manual/hg-manual.html)
- [Coffman Conditions — Wikipedia](https://en.wikipedia.org/wiki/Deadlock#Coffman_conditions)

**AI usage:** Claude (Anthropic) was used to generate this README from the source code and project subject.