#include "../minishell.h"
#include "../gnl/get_next_line.h"

int	is_tvnext(t_token *token, t_type type)
{
	if (!token)
		return (0);
	if (!token->next)
		return (0);
	if (!token->next->value[1])
		return (0);
	if (token->next->type != type)
		return (0);
	return (1);
}

int	is_tnext(t_token *token, t_type type)
{
	if (!token)
		return (0);
	if (!token->next)
		return (0);
	if (token->next->type != type)
		return (0);
	return (1);
}

int	do_here_doc(t_ms *head, t_token *token, char *path_doc)
{
	if (token->type == _redirection && token->value[0][0] == '<')
	{
		redirection_in(token);
		head->token_count += 1;
		token = token->next;
	}
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
}

int	multi_commands(t_ms *head)
{
	char	*path_doc;
	t_token	*tk;
	t_token	*tkn;

	creat_needed_files(head->tokens);
	tk = get_n_token(head->tokens, head->token_count);
	path_doc = NULL;
	if (do_here_doc(head, tk, path_doc))
		return (0);
	while (tk)
	{
		tkn = tk->next;
		if (tk->type == _cmd_grp && is_tnext(tk, _pipe) && is_tnext(tkn, _cmd_grp))   
			pids_addback(&head->pids, pipe_and_exec(head, tk, path_doc, 0));
		else if (tk->type == _cmd_grp && is_tnext(tk, _redirection) && tkn->value[0][0] == '>')
			pids_addback(&head->pids, redirection_out(head, tk));
		else if (tk->type == _cmd_grp && is_tnext(tk, _append))
			pids_addback(&head->pids, redirection_out(head, tk));
		else if (tk->type == _cmd_grp)
			pids_addback(&head->pids, pipe_and_exec(head, tk, path_doc, 1));
		head->token_count += 1;
		tk = tk->next;
	}
	return (0);
}

void	command_manager(t_ms *head)
{
	int		original_stdint;
	t_pids	*tmp;

	original_stdint = dup(STDIN_FILENO);
	sig_control(0);
	multi_commands(head);
	tmp = head->pids;
	while (tmp)
	{
		if (tmp->pid > 0)
		{
			if (g_sig_received)
				kill(tmp->pid, g_sig_received);
			waitpid(tmp->pid, NULL, 0);
		}
		tmp = tmp->next;
	}
	pids_clear(head->pids);
	head->pids = NULL;
	g_sig_received = 0;
	sig_control(1);
	dup2(original_stdint, STDIN_FILENO);
	close(original_stdint);
}
