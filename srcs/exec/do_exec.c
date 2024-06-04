/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   do_exec.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymarques <ymarques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 11:39:31 by ymarques          #+#    #+#             */
/*   Updated: 2024/06/04 11:32:47 by ymarques         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../gnl/get_next_line.h"

int	do_pipe_error(t_ms *head, t_token **token)
{
	t_token	*tk;

	tk = *token;
	if (is_file_error_in_pipe(tk))
	{
		pids_addback(&head->pids, -42);
		if (tk->type == _pipe)
			tk = tk->next;
		while (tk && tk->type != _pipe)
			tk = tk->next;
		if (tk)
		{
			rd_null(head);
			tk = get_next_pipe(tk);
		}
		*token = tk;
		return (1);
	}
	return (0);
}

int	do_cmd_and_rd(t_ms *head, t_token **tk)
{
	if (is_cmd_rdout(*tk))
	{
		pids_addback(&head->pids, pipe_and_exec(head, *tk, 0));
		do_redirection_out(head, (*tk)->next);
		if (!set_tk_at_next_cmd(tk))
			*tk = NULL;
		return (1);
	}
	if (is_cmd_rdin(*tk) || is_cmd_heredoc(*tk))
	{
		do_redirection_in(head, (*tk)->next);
		if (have_next_pipe(*tk) || next_redirect_out((*tk)->next))
			pids_addback(&head->pids, pipe_and_exec(head, *tk, 0));
		else
			pids_addback(&head->pids, pipe_and_exec(head, *tk, 1));
		set_tk_at_after_stdin(tk);
		return (1);
	}
	return (0);
}

int	set_tk_at_after_stdout(t_token **token)
{
	t_token	*tmp;

	tmp = (*token)->next;
	while (tmp)
	{
		if (!is_rdout(tmp))
		{
			*token = tmp;
			return (1);
		}
		tmp = tmp->next;
	}
	if (!tmp)
		*token = NULL;
	return (0);
}

int	do_rd(t_ms *head, t_token **tk)
{
	if (is_rdin(*tk) || is_heredoc(*tk))
	{
		do_redirection_in(head, *tk);
		set_tk_at_after_stdin(tk);
		return (1);
	}
	if (is_rdout(*tk))
	{
		if ((*tk)->prev)
		{
			do_redirection_out(head, *tk);
			set_tk_at_after_stdout(tk);
			return (1);
		}
	}
	return (0);
}
