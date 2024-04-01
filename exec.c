#include "minishell.h"

int is_builtin(char *cmd)
{
    if (strcmp(cmd, "echo"))
        return (1);
    if (strcmp(cmd, "cd"))
        return (1);
    if (strcmp(cmd, "pwd"))
        return (1);
    if (strcmp(cmd, "export"))
        return (1);
    if (strcmp(cmd, "unset"))
        return (1);
    if (strcmp(cmd, "env"))
        return (1);
    if (strcmp(cmd, "exit"))
        return (1);
    return (0);
}

int token_len(t_token *tokens)
{
	t_token	*tmp;
	int		i;

	i = 0;
	tmp = tokens;
	while (tmp)
	{
		tmp = tmp->next;
		++i;
	}
	return (i);
}


char *find_path(t_ms *head)
{
    char    **paths;
    char    *path;
	char	*path_command;
    int     i;

    if (access(head->tokens->value[0], F_OK) != -1)
        return (head->tokens->value[0]);
    i = 0;
    path = get_var_value(head->env, "PATH");
	if (!path)
		return (NULL);
    paths = ft_split(path, ':');
    i = 0;
    while (paths[i])
	{
		path = ft_strjoin(paths[i++], "/");
		path_command = ft_strjoin(path, head->tokens->value[0]);
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

int execute(t_ms *head)
{
    char *path;

    path = find_path(head);
    if (!path)
        return (-1);
    if (execve(path, head->tokens->value, NULL) == -1)
    {
        free(path);
        perror("error");
    	return (-1);
    }
    free(path);
   	return (0);
}

void    error_exit(char *str)
{
    printf("%s", str);
    exit(EXIT_FAILURE);
}

void    launch_process(t_ms *head)
{
	int	pid;
	int	fd[2];

	if (pipe(fd) == -1)
		error_exit("Error with the pipe");
	pid = fork();
	if (pid == -1)
		error_exit("Error with the pipe");
	if (pid == 0)
	{
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
		if (execute(head) == -1)
			exit(2);
	}
	else
	{
		waitpid(pid, NULL, 0);
		close(fd[1]);
		dup2(fd[0], STDIN_FILENO);
	}
}

int multi_commands(t_ms *head)
{
    t_token *tmp;

    tmp = head->tokens;
    while (tmp)
    {
        if (tmp->type == _cmd_grp)
            launch_process(head);
        tmp = tmp->next;
    }
    return (0);
}

void	command_manager(t_ms *head)
{
	int	saved_stdout;
	int	saved_stdin;

	if (ft_strcmp(head->tokens->value[0], "exit") == 0)
		exit(1);
	saved_stdout = dup(STDOUT_FILENO);
	saved_stdin = dup(STDIN_FILENO);
	multi_commands(head);
	dup2(saved_stdout, STDOUT_FILENO);
	dup2(saved_stdin, STDIN_FILENO);
}
