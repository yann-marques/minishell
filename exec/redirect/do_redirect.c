/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   do_redirect.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymarques <ymarques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 11:39:31 by ymarques          #+#    #+#             */
/*   Updated: 2024/05/08 16:06:41 by ymarques         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"
#include "../../gnl/get_next_line.h"

int	do_redirection_out(t_ms *head, t_token *tk)
{
	t_token	*tmp;
	int		fd_out;

	(void) head;
	tmp = tk;
	fd_out = -1;
	while (tmp)
	{
		if ((tmp->type == _redirection && tmp->value[0][0] == '>')
			|| tmp->type == _append)
		{
			fd_out = check_file_out(tmp);
			if (fd_out == -1)
			{
				pids_addback(&head->pids, -42);
				return (perror_str(" ", -1));
			}
		}
		else
			break ;
		tmp = tmp->next;
	}
	if (fd_out != -1)
		redirection_out(fd_out);
	return (1);
}

int	do_redirection_in(t_ms *head, t_token *tk)
{
	t_token	*tmp;

	(void) head;
	tmp = tk;
	while (tmp)
	{
		if (tmp->type == _redirection && tmp->value[0][0] == '<')
			redirection_in(tmp);
		else
			break ;
		tmp = tmp->next;
	}
	return (1);
}
