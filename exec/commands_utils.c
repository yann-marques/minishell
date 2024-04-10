#include "../minishell.h"
#include "../gnl/get_next_line.h"

static void	error_exit(char *str)
{
	printf("%s", str);
	exit(EXIT_FAILURE);
}

char	*find_path(t_ms *head, t_token *token)
{
	char	**paths;
	char	*path;
	char	*path_command;
	int		i;

	if (access(token->value[0], F_OK) != -1)
		return (token->value[0]);
	i = 0;
	path = get_var_value(head->env, "PATH");
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
		return (-1);
	env = t_env_to_strtab(head->env);
	if (!env)
		return (-1);
	if (execve(path, token->value, env) == -1)
	{
		free(path);
		strtab_clear(env);
		perror("error");
		return (-1);
	}
	free(path);
	return (0);
}

int	pipe_and_exec(t_ms *head, t_token *token, char *path_doc, int last_command)
{
	int		pid;
	int		tmp_fd;
	int		fd[2];

	if (pipe(fd) == -1)
		error_exit("Error with the pipe");
	pid = fork();
	if (pid == -1)
		error_exit("Error with the pipe");
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
		if (is_builtin(head, token) != 1)
			exit(2);
		if (execute(head, token) == -1)
			exit(2);
	}
	else
	{
		if (!last_command)
			dup2(fd[0], STDIN_FILENO);
		close(fd[1]);
		close(fd[0]);
	}
	return (pid);
}

int	check_echo_builtin(char **value)
{
	int	i;

	i = 0;
	if (ft_strcmp(value[0], "echo") != 0)
		return (0);
	if (!value[1])
		return (0);
	if (value[1][i++] != '-')
		return (0);
	if (value[1][i++] != 'n')
		return (0);
	while (value[1][i])
	{
		if (value[1][i] != 'n')
			return (0);
		++i;
	}
	return (1);
}

int	is_builtin(t_ms *head, t_token *token)
{
	if (check_echo_builtin(token->value))
		return (ms_echo_n(token));
	if (ft_strcmp(token->value[0], "cd") == 0)
		return (ms_cd(token));
	if (ft_strcmp(token->value[0], "pwd") == 0)
		return (ms_pwd());
	if (ft_strcmp(token->value[0], "export") == 0)
		return (ms_export(head->env, token));
	if (ft_strcmp(token->value[0], "unset") == 0)
		return (ms_unset(head->env, token));
	if (ft_strcmp(token->value[0], "env") == 0 && !token->value[1])
		return (ms_env(head->env, NULL));
	if (ft_strcmp(token->value[0], "exit") == 0)
		return (ms_exit(head));
	return (1);
}