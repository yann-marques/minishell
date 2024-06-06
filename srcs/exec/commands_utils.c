/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymarques <ymarques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 11:39:31 by ymarques          #+#    #+#             */
/*   Updated: 2024/06/06 16:25:58 by ymarques         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../gnl/get_next_line.h"

static void	exit_error_12x(t_ms *head, t_token *token)
{
	if (token->value[0][0] == '/' || (token->value[0][0] == '.'
		&& token->value[0][1] == '/'))
	{
		if (is_file(token->value[0]) == 0)
			error_exit(head, " Is a directory", 126);
		if (access(token->value[0], F_OK) != 0)
			perror_exit(head, token->value[0], 127);
		if (access(token->value[0], R_OK) != 0)
			perror_exit(head, token->value[0], 126);
		if (access(token->value[0], X_OK) != 0)
			perror_exit(head, token->value[0], 127);
	}
}

int	execute(t_ms *head, t_token *token)
{
	char	**env;
	char	*path;

	exit_error_12x(head, token);
	path = find_path(head, token);
	if (!path)
	{
		error_str(token->value[0]);
		error_exit(head, ": command not found", 127);
	}
	env = t_env_to_strtab(head->env);
	if (!env)
		perror_exit(head, " ", EXIT_FAILURE);
	if (execve(path, token->value, env) == -1)
	{
		free(path);
		strtab_clear(env);
		error_str(token->value[0]);
		error_exit(head, ": command not found", 127);
	}
	return (0);
}

static void	redirect_if_lastcommand(int pid, int *fd, int last_command)
{
	if (pid == 0)
	{
		if (!last_command)
			dup2(fd[1], STDOUT_FILENO);
		close(fd[0]);
		close(fd[1]);
	}
	else
	{
		if (!last_command)
			dup2(fd[0], STDIN_FILENO);
		else
			close(STDIN_FILENO);
		close(fd[1]);
		close(fd[0]);
	}
}

int	pipe_and_exec(t_ms *head, t_token *token, int last_command)
{
	int		pid;
	int		fd[2];

	if (check_if_builtins_parent(head, token))
		return (0);
	if (pipe(fd) == -1)
		return (-1);
	pid = fork();
	if (pid == -1)
		return (-1);
	if (pid == 0)
	{
		close(head->original_stdint);
		close(head->heredoc_stdint);
		redirect_if_lastcommand(pid, fd, last_command);
		if (builtin_child(head, token))
			exit_free_head(head, 1);
		execute(head, token);
	}
	else
		redirect_if_lastcommand(pid, fd, last_command);
	return (pid);
}
