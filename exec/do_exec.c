/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   do_exec.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymarques <ymarques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 11:39:31 by ymarques          #+#    #+#             */
/*   Updated: 2024/05/20 14:53:36 by ymarques         ###   ########.fr       */
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
		perror_str(" ", -1);
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

int	do_heredoc(t_ms *head, t_token **token, char **path_doc)
{
	t_token	*tk;

	tk = *token;
	if (is_cmd_heredoc(tk))
	{
		*path_doc = here_doc(head, tk->next);
		if (have_next_pipe(tk) || next_redirect_out(tk->next))
			pids_addback(&head->pids, pipe_and_exec(head, tk, path_doc, 0));
		else
			pids_addback(&head->pids, pipe_and_exec(head, tk, path_doc, 1));
		*path_doc = NULL;
		if (tk->next)
			tk = tk->next->next;
		*token = tk;
		return (1);
	}
	if (is_heredoc(tk))
	{
		*path_doc = here_doc(head, tk);
		tk = tk->next;
		*token = tk;
		return (1);
	}
	return (0);
}

int	do_cmd_and_rd(t_ms *head, t_token **tk, char **path_doc)
{
	if (is_cmd_rdout(*tk))
	{
		pids_addback(&head->pids, pipe_and_exec(head, *tk, path_doc, 0));
		do_redirection_out(head, (*tk)->next);
		if (!set_tk_at_next_cmd(*tk))
			*tk = NULL;
		return (1);
	}
	if (is_cmd_rdin(*tk))
	{
		do_redirection_in(head, (*tk)->next);
		if (have_next_pipe(*tk) || next_redirect_out((*tk)->next))
			pids_addback(&head->pids, pipe_and_exec(head, *tk, path_doc, 0));
		else
			pids_addback(&head->pids, pipe_and_exec(head, *tk, path_doc, 1));
		if ((*tk)->next->next)
			*tk = (*tk)->next->next;
		else
			*tk = NULL;
		return (1);
	}
	return (0);
}

int	do_rd(t_ms *head, t_token **tk)
{
	if (is_rdin(*tk))
	{
		do_redirection_in(head, *tk);
		if ((*tk)->next)
		{
			*tk = (*tk)->next;
			return (1);
		}
	}
	if (is_rdout(*tk))
	{
		if ((*tk)->prev && ((*tk)->prev->type == _cmd_grp
				|| is_cmd_rdin((*tk)->prev->prev)
				|| is_cmd_heredoc((*tk)->prev->prev)))
		{
			do_redirection_out(head, *tk);
			if ((*tk)->next)
			{
				*tk = (*tk)->next;
				return (1);
			}
		}
	}
	return (0);
}
