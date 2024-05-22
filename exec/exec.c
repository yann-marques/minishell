/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymarques <ymarques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 11:39:31 by ymarques          #+#    #+#             */
/*   Updated: 2024/05/22 11:18:20 by ymarques         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../gnl/get_next_line.h"

int	set_tk_at_next_cmd(t_token *token)
{
	t_token	*tmp;

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

int	multi_commands(t_ms *head)
{
	char	*path_doc;
	t_token	*tk;

	tk = head->tokens;
	do_needed_files(tk);
	move_rdout(&tk);
	path_doc = NULL;
	while (tk)
	{
		if (do_pipe_error(head, &tk))
			continue ;
		if (do_heredoc(head, &tk, &path_doc))
			continue ;
		if (do_cmd_and_rd(head, &tk, &path_doc))
			continue ;
		if (do_rd(head, &tk))
			continue ;
		if (is_cmd(tk) && have_next_pipe(tk))
			pids_addback(&head->pids, pipe_and_exec(head, tk, &path_doc, 0));
		if (is_cmd(tk) && !have_next_pipe(tk) && !next_redirect(tk))
			pids_addback(&head->pids, pipe_and_exec(head, tk, &path_doc, 1));
		tk = tk->next;
	}
	return (0);
}

static void	process_pids(t_ms *head, int pid)
{
	int	status;

	status = 0;
	if (pid > 0)
	{
		if (g_sig_received)
			kill(pid, g_sig_received);
		waitpid(pid, &status, 0);
		if (WTERMSIG(status) == SIGQUIT)
			error_str(" Quit (Core dumped)\n");
		if (WTERMSIG(status) == SIGPIPE)
			error_str(" Broken pipe\n");
		if (WIFEXITED(status))
			head->last_status = WEXITSTATUS(status);
		if (WIFSIGNALED(status))
			head->last_status = WTERMSIG(status) + 128;
	}
	if (pid == -42)
		head->last_status = EXIT_FAILURE;
}

void	command_manager(t_ms *head)
{
	int		original_stdint;
	t_pids	*pids;

	original_stdint = dup(STDIN_FILENO);
	head->original_stdint = original_stdint;
	sig_control(0);
	multi_commands(head);
	pids = head->pids;
	while (pids)
	{
		process_pids(head, pids->pid);
		pids = pids->next;
	}
	pids_clear(head->pids);
	head->pids = NULL;
	g_sig_received = 0;
	sig_control(1);
	dup2(original_stdint, STDIN_FILENO);
	close(original_stdint);
}
