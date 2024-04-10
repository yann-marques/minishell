#include "../minishell.h"
#include "../gnl/get_next_line.h"

int	multi_commands(t_ms *head)
{
	char	*path_doc;
	t_token	*token;

	creat_needed_files(head->tokens);
	token = get_n_token(head->tokens, head->token_count);
	path_doc = NULL;
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
	if (token->type == _cmd_grp && token->next && token->next->type == _delimiter && token->next->value[1])
	{
		path_doc = here_doc(head, token->next);
		if (!token->next->next)
			return (pids_addback(&head->pids, pipe_and_exec(head, token, path_doc, 1)));
		pids_addback(&head->pids, pipe_and_exec(head, token, path_doc, 0));
		head->token_count += 3;
		token = token->next->next->next;
	}
	while (token)
	{
		if (token->type == _cmd_grp && token->next && token->next->type == _pipe && token->next->next && token->next->next->type == _cmd_grp)
			pids_addback(&head->pids, pipe_and_exec(head, token, path_doc, 0));
		else if (token->type == _cmd_grp && token->next && token->next->type == _redirection && token->next->value[0][0] == '>' && token->next->value[1])
			pids_addback(&head->pids, redirection_out(head, token));
		else if (token->type == _cmd_grp && token->next && token->next->type == _append && token->next->value[1])
			pids_addback(&head->pids, redirection_out(head, token));
		else if (token->type == _cmd_grp)
			pids_addback(&head->pids, pipe_and_exec(head, token, path_doc, 1));
		head->token_count += 1;
		token = token->next;
	}
	return (0);
}

void	command_manager(t_ms *head)
{
	int		original_stdint;
	t_pids	*tmp;

	if (ft_strcmp(head->tokens->value[0], "exit") == 0)
		ms_exit(head);
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
