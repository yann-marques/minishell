#include "../minishell.h"
#include "../gnl/get_next_line.h"

char	*find_path(t_ms *head, t_token *token)
{
	char	**paths;
	char	*path;
	char	*path_command;
	int		i;

	if (access(token->value[0], F_OK) != -1)
		return (token->value[0]);
	i = 0;
	path = get_var_value(head, "PATH");
	if (!path)
		return (NULL);
	paths = ft_split(path, ':');
	i = 0;
	while (paths[i])
	{
		path = ft_strjoin(paths[i++], "/");
		path_command = ft_strjoin(path, token->value[0]);
		free(path);
		if (access(path_command, F_OK) == 0)
		{
			strtab_clear(paths);
			return (path_command);
		}
		else
			free(path_command);
	}
	strtab_clear(paths);
	return (NULL);
}

int	execute(t_ms *head, t_token *token)
{
	char	**env;
	char	*path;

	path = find_path(head, token);
	if (!path)
	{
		perror(" ");
		exit(EXIT_FAILURE);
	}
	env = t_env_to_strtab(head->env);
	if (!env)
	{
		perror(" ");
		exit(EXIT_FAILURE);
	}
	if (execve(path, token->value, env) == -1)
	{
		free(path);
		strtab_clear(env);
		perror(" ");
		exit(EXIT_FAILURE);
	}
	return (0);
}

int	pipe_and_exec(t_ms *head, t_token *token, char *path_doc, int last_command)
{
	int		pid;
	int		tmp_fd;
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
		if (path_doc && access(path_doc, F_OK) == 0)
		{
			tmp_fd = open(path_doc, O_RDONLY, 0644);
			if (tmp_fd == -1)
				error_exit("Error with path_doc");
			dup2(tmp_fd, STDIN_FILENO);
			unlink(path_doc);
			close(tmp_fd);
		}
		if (!last_command)
		{
			dup2(fd[1], STDOUT_FILENO);
			close(fd[0]);
			close(fd[1]);
		}
		if (builtin_child(head, token))
			exit(head->last_status);
		execute(head, token);
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
	return (pid);
}
