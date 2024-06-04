/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   do_redirect.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymarques <ymarques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 11:39:31 by ymarques          #+#    #+#             */
/*   Updated: 2024/06/04 13:34:57 by ymarques         ###   ########.fr       */
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
				return (perror_str(tk->value[0], -1));
		}
		else
			break ;
		if (tmp->next && is_rdout(tmp->next))
			close(fd_out);
		tmp = tmp->next;
	}
	if (fd_out != -1)
		redirection_out(fd_out);
	return (1);
}

int	do_redirection_in(t_ms *head, t_token *tk)
{
	t_token	*tmp;
	int		fd;

	tmp = tk;
	fd = -1;
	while (tmp)
	{
		if (tmp->type == _redirection && tmp->value[0][0] == '<')
			fd = redirection_in(head, tmp);
		else if (is_heredoc(tmp))
			fd = here_doc(head, tmp);
		else
			break ;
		if (fd == -1)
			break ;
		tmp = tmp->next;
		if (tmp && fd != -1 && ((tmp->type == _redirection
					&& tmp->value[0][0] == '<') || is_heredoc(tmp)))
			close(fd);
	}
	if (fd != -1)
		dup2(fd, STDIN_FILENO);
	else
		rd_null(head);
	if (fd != -1)
		close(fd);
	return (1);
}
