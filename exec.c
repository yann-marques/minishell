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
    int     i;

    if (access(head->tokens->value[0], F_OK) != -1)
        return (head->tokens->value[0]);
    i = 0;
    path = get_var_value(head->env, "PATH");
	if (!path)
		return (NULL);
    paths = ms_split(path, ":");
    i = 0;
    while (paths && paths[i])
    {
        path = ms_join_three(ft_strndup(paths[i], 0), NULL, ft_strndup("/", 0));
        path = ms_join_three(path, NULL, ft_strndup(head->tokens->value[0], 0));
        if (!path)
            break ;
        if (access(path, F_OK) != -1)
        {
            strtab_clear(paths);
            return (path);
        }
        free(path);
        i++;
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
        exit(0);
    }
    free(path);
    exit(1);
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
	send_head(head);
	pid = fork();
	if (pid == -1)
		error_exit("Error with the pipe");
	if (pid == 0)
	{
		send_head(NULL);
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
		execute(head);
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
