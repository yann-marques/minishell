/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymarques <ymarques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 11:39:31 by ymarques          #+#    #+#             */
/*   Updated: 2024/06/06 16:33:51 by ymarques         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../gnl/get_next_line.h"

int	set_tk_at_next_cmd(t_token **token)
{
	t_token	*tmp;

	if (!have_next_pipe(*token))
		return (0);
	tmp = (*token)->next;
	while (tmp)
	{
		if (tmp->type == _cmd_grp)
		{
			*token = tmp;
			return (1);
		}
		tmp = tmp->next;
	}
	return (0);
}

int	set_tk_at_after_stdin(t_token **token)
{
	t_token	*tmp;

	tmp = (*token)->next;
	while (tmp)
	{
		if (!is_rdin(tmp) && !is_heredoc(tmp))
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

int	multi_commands(t_ms *head)
{
	t_token	*tk;

	tk = head->tokens;
	do_needed_files(tk);
	move_rdout(&tk);
	while (tk)
	{
		if (do_pipe_error(head, &tk))
			continue ;
		if (do_cmd_and_rd(head, &tk))
			continue ;
		if (do_rd(head, &tk))
			continue ;
		if (is_cmd(tk) && have_next_pipe(tk))
			pids_addback(&head->pids, pipe_and_exec(head, tk, 0));
		if (is_cmd(tk) && !have_next_pipe(tk) && !next_redirect(tk))
			pids_addback(&head->pids, pipe_and_exec(head, tk, 1));
		tk = tk->next;
	}
	return (0);
}

static void	process_pids(t_ms *head, int pid, int last_pid)
{
	int	status;

	status = 0;
	if (pid > 0)
	{
		if (g_sig_received)
			kill(pid, g_sig_received);
		waitpid(pid, &status, 0);
		if (WTERMSIG(status) == SIGQUIT && last_pid)
			error_str(" Quit (Core dumped)\n");
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
	t_pids	*pids;

	head->original_stdint = dup(STDIN_FILENO);
	head->heredoc_stdint = dup(STDIN_FILENO);
	sig_control(0);
	multi_commands(head);
	pids = head->pids;
	while (pids)
	{
		if (!pids->next)
			process_pids(head, pids->pid, 1);
		else
			process_pids(head, pids->pid, 0);
		pids = pids->next;
	}
	if (g_sig_received == SIGINT)
		head->last_status = SIGINT + 128;
	pids_clear(head->pids);
	head->pids = NULL;
	g_sig_received = 0;
	sig_control(1);
	dup2(head->original_stdint, STDIN_FILENO);
	close(head->original_stdint);
	close(head->heredoc_stdint);
}
