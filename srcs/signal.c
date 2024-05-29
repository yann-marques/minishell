/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yanolive <yanolive@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 17:13:06 by yanolive          #+#    #+#             */
/*   Updated: 2024/05/29 16:47:25 by yanolive         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_fork(int signum);
static void	handler_sigint(int signum);

int	g_sig_received;

void	sig_control(int handle)
{
	if (handle)
	{
		signal(SIGINT, handler_sigint);
		signal(SIGQUIT, SIG_IGN);
	}
	else
	{
		signal(SIGINT, handle_fork);
		signal(SIGQUIT, handle_fork);
	}
}

static void	handle_fork(int signum)
{
	g_sig_received = signum;
	printf("\n");
}

static void	handler_sigint(int signum)
{
	g_sig_received = signum;
	printf("\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}
