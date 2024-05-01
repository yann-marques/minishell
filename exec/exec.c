#include "../minishell.h"
#include "../gnl/get_next_line.h"

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

int	have_next_redi(t_token *token)
{
	t_token *tmp;

	if (token->next)
		tmp = token->next;
	else
		return (0);
	while (tmp)
	{
		if (tmp->type == _append)
			return (1);
		if (tmp->type == _redirection)
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

int	next_redirect_out(t_token *tk)
{
	if (!tk->next)
		return (0);
	if ((tk->next->type == _redirection && tk->next->value[0][0] == '>') || tk->next->type == _append)
		return (1);
	return (0);
}

int	multi_commands(t_ms *head)
{
	char	*path_doc;
	t_token	*tk;
	int		infile;

	tk = head->tokens;
	do_needed_files(tk);
	path_doc = NULL;
	while (tk)
	{
		if (is_file_error_in_pipe(tk))
		{
			pids_addback(&head->pids, perror_str(" ", -42));
			if (tk->type == _pipe)
				tk = tk->next;
			while(tk && tk->type != _pipe)
				tk = tk->next;
			if (tk)
			{
				infile = open("/dev/null", O_RDONLY, 0644);
				if (infile == -1)
					perror_exit(" ", EXIT_FAILURE);
				dup2(infile, STDIN_FILENO);
				close(infile);
				tk = get_next_pipe(tk);
			}
			continue ;
		}
		if (is_cmd_heredoc(tk))
		{
			path_doc = here_doc(head, tk->next);
			if (have_next_pipe(tk))
				pids_addback(&head->pids, pipe_and_exec(head, tk, path_doc, 0));
			else
				pids_addback(&head->pids, pipe_and_exec(head, tk, path_doc, 1));
			if (tk->next)
				tk = tk->next->next;
			continue;
		}
		if (is_heredoc(tk))
		{
			path_doc = here_doc(head, tk);
			tk = tk->next;
			continue ;
		}
		if (is_cmd_rdout(tk))
		{
			pids_addback(&head->pids, pipe_and_exec(head, tk, path_doc, 0));
			do_redirection_out(head, tk->next);
			if (!set_tk_at_next_cmd(tk))
				return (1);
			else
				continue ;
		}
		if (is_cmd_rdin(tk))
		{
			do_redirection_in(head, tk->next);
			if (have_next_pipe(tk) || next_redirect_out(tk->next))
				pids_addback(&head->pids, pipe_and_exec(head, tk, path_doc, 0));
			else
				pids_addback(&head->pids, pipe_and_exec(head, tk, path_doc, 1));
			if (tk->next)
				tk = tk->next->next;
			continue;
		}
		if (is_rdin(tk))
		{
			do_redirection_in(head, tk);
			if (tk->next)
			{
				tk = tk->next;
				continue ;
			}
		}
		if (is_rdout(tk))
		{
			do_redirection_out(head, tk);
			if (tk->next)
			{
				tk = tk->next;
				continue ;
			}
		}
		if (is_cmd(tk) && have_next_pipe(tk))
			pids_addback(&head->pids, pipe_and_exec(head, tk, path_doc, 0));
		if (is_cmd(tk) && !have_next_pipe(tk) && !next_redirect(tk))
			pids_addback(&head->pids, pipe_and_exec(head, tk, path_doc, 1));
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
			if (WIFSIGNALED(status))
			{
				if (WTERMSIG(status) == SIGPIPE)
					error_str(" Broken pipe\n");
				if (WTERMSIG(status) == SIGQUIT)
					error_str(" Quit (Core dumped)\n");
				head->last_status = WTERMSIG(status) + 128;
			}
			else if (WIFEXITED(status))
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
