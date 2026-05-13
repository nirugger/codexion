/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nirugger <nirugger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 19:08:35 by nirugger          #+#    #+#             */
/*   Updated: 2026/05/13 20:13:28 by nirugger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	init_msg_struct(t_msg *msgs)
{
	msgs->dongle = "has taken a dongle";
	msgs->burnout = "burned out";
	msgs->compile = "is compiling";
	msgs->debug = "is debugging";
	msgs->refactor = "is refactoring";
}