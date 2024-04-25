#include "../minishell.h"
#include "../gnl/get_next_line.h"

int	is_rdin(t_token *tk)
{
	if (tk->type == _redirection && tk->value[0][0] == '<' && !tk->value[0][1])
		return (1);
	return (0);
}

int	is_rdout(t_token *tk)
{
	if (tk->type == _redirection && tk->value[0][0] == '>')
		return (1);
	if (tk->type == _append)
		return (1);
	return (0);
}
int	is_cmd_rdout(t_token *tk)
{
	if (tk->type == _cmd_grp && tk->next && is_rdout(tk->next))
		return (1);
	return (0);
}

int	is_cmd_rdin(t_token *tk)
{
	if (tk->type == _cmd_grp && tk->next && is_rdin(tk->next))
		return (1);
	return (0);
}

int	is_cmd(t_token *tk)
{
	if (tk->type == _cmd_grp)
		return (1);
	return (0);
}


/* int	do_here_doc(t_ms *head, t_token *token, char *path_doc)
{
	if (token->type == _delimiter && token->value[1])
	{
		path_doc = here_doc(head, token);
		head->token_count += 1;
		token = token->next;
	}
	if (token->type == _cmd_grp && is_tvnext(token, _delimiter))
	{
		path_doc = here_doc(head, token->next);
		if (!token->next->next)
		{
			pids_addback(&head->pids, pipe_and_exec(head, token, path_doc, 1));
			return (1);
		}
		pids_addback(&head->pids, pipe_and_exec(head, token, path_doc, 0));
		head->token_count += 3;
		token = token->next->next->next;
	}
	return (0);
} */

int	have_next_pipe(t_token *token)
{
	t_token *tmp;

	if (token->next)
		tmp = token->next;
	else
		return (0);
	while (tmp)
	{
		if (tmp->type == _pipe)
			return (1);
		tmp = tmp->next;
	}
	return (0);
}

int	set_tk_at_next_cmd(t_token *token)
{
	t_token *tmp;

	if (!have_next_pipe(token))
		return (0);
	if (token->next)
		tmp = token->next;
	else
		return (0);
	while (tmp)
	{
		if (tmp->type == _cmd_grp)
		{
			*token = *tmp;
			return (1);
		}
		tmp = tmp->next;
	}
	return (0);
}

int	next_redirect(t_token *tk)
{
	if (!tk->next)
		return (0);
	if (tk->next->type == _redirection || tk->next->type == _append)
		return (1);
	return (0);
}

int	multi_commands(t_ms *head)
{
	//char	*path_doc;
	t_token	*tk;

	tk = get_n_token(head->tokens, head->token_count);
	do_needed_files(head);
	while (tk)
	{
		if (is_cmd_rdout(tk))
		{
			pids_addback(&head->pids, pipe_and_exec(head, tk, NULL, 0));
			if (do_redirection_out(head, tk->next) == -42)
			{
				pids_addback(&head->pids, -42);
				if (!set_tk_at_next_cmd(tk))
					return (1);
				else
					continue ;
			}
			if (!set_tk_at_next_cmd(tk))
				return (1);
			else
				continue ;
		}
		if (is_cmd_rdin(tk))
		{
			if (do_redirection_in(head, tk->next) == -42)
			{
				pids_addback(&head->pids, -42);
				if (!set_tk_at_next_cmd(tk))
					return (1);
				else
					continue ;
			}
			if (have_next_pipe(tk))
				pids_addback(&head->pids, pipe_and_exec(head, tk, NULL, 0));
			else
				pids_addback(&head->pids, pipe_and_exec(head, tk, NULL, 1));
			if (!set_tk_at_next_cmd(tk))
				return (1);
			else
				continue ;
		}
		if (is_rdin(tk))
			do_redirection_in(head, tk);
		if (is_rdout(tk))
			do_redirection_out(head, tk);
		if (is_cmd(tk) && have_next_pipe(tk))
			pids_addback(&head->pids, pipe_and_exec(head, tk, NULL, 0));
		if (is_cmd(tk) && !have_next_pipe(tk) && !next_redirect(tk))
			pids_addback(&head->pids, pipe_and_exec(head, tk, NULL, 1));
		tk = tk->next;
	}
	return (0);
}

void	command_manager(t_ms *head)
{
	int		original_stdint;
	int		status;
	t_pids	*tmp;

	original_stdint = dup(STDIN_FILENO);
	sig_control(0);
	multi_commands(head);
	tmp = head->pids;
	status = 0;
	while (tmp)
	{
		if (tmp->pid > 0)
		{
			if (g_sig_received)
				kill(tmp->pid, g_sig_received);
			waitpid(tmp->pid, &status, 0);
			if (WIFEXITED(status))
				head->last_status = WEXITSTATUS(status);
		}
		if (tmp->pid == -42)
			head->last_status = EXIT_FAILURE;
		tmp = tmp->next;
	}
	pids_clear(head->pids);
	head->pids = NULL;
	g_sig_received = 0;
	sig_control(1);
	dup2(original_stdint, STDIN_FILENO);
	close(original_stdint);
}
