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


char *find_path(t_ms *head, t_token *token)
{
    char    **paths;
    char    *path;
	char	*path_command;
    int     i;

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

int	env_size(t_env *env)
{
	t_env	*tmp;
	int		count;

	tmp = env;
	count = 0;
	while (tmp)
	{
		tmp = tmp->next;
		++count;
	}
	return (count);
}

char	**t_env_to_strtab(t_env *env)
{
	char	**envp;
	t_env	*tmp_env;
	char	*tmp;
	int		k;

	envp = malloc(sizeof(char *) * (env_size(env) + 1));
	if (!envp)
		return (NULL);
	tmp_env = env;
	k = 0;
	while (tmp_env)
	{
		tmp = ft_strjoin(tmp_env->var, "=");
		envp[k] = ft_strjoin(tmp, tmp_env->value);
		if (tmp)
			free(tmp);
		if (!envp[k])
		{
			strtab_clear(envp);
			return (NULL);
		}
		++k;
		tmp_env = tmp_env->next; 
	}
	return (envp);
}

int execute(t_ms *head, t_token *token)
{
	char	**env;
    char 	*path;

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

void    error_exit(char *str)
{
    printf("%s", str);
    exit(EXIT_FAILURE);
}

void    pipe_and_exec(t_ms *head, t_token *token, int last_command)
{
	int		pid;
	int		fd[2];

	if (pipe(fd) == -1)
		error_exit("Error with the pipe");
	pid = fork();
	if (pid == -1)
		error_exit("Error with the pipe");
	if (pid == 0)
	{
		if (!last_command)
		{
			dup2(fd[1], STDOUT_FILENO);
			close(fd[0]);
			close(fd[1]);
		}
		if (execute(head, token) == -1)
			exit(2);
	}
	else
	{
		waitpid(pid, NULL, 0);
		close(fd[1]);
		if (!last_command)
			dup2(fd[0], STDIN_FILENO);
	}
	return ;
}

t_token	*get_n_token(t_token *tokens, int count)
{
	t_token	*tmp;
	int		i;

	tmp = tokens;
	i = 0;
	while (tmp && i < count)
	{
		tmp = tmp->next;
		++i;
	}
	return (tmp);
}

/* void	redirection(t_ms *head, t_token *token)
{
0
} */

int multi_commands(t_ms *head)
{
    t_token *token;

    token = get_n_token(head->tokens, head->token_count);
    while (token)
    {
        if (token->type == _cmd_grp && token->next && token->next->type == _pipe && token->next->next && token->next->next->type == _cmd_grp)
            pipe_and_exec(head, token, 0);
		else if (token->type == _cmd_grp && token->next && token->next->type == _redirection && token->next->value[0][0] == '>' && token->next->next->type == _file)
			pipe_and_exec(head, token, 1);
		else if (token->type == _cmd_grp)
			pipe_and_exec(head, token, 1);
		head->token_count += 1;
        token = token->next;
    }
    return (0);
}

void	command_manager(t_ms *head)
{
	int original_stdint;
	
	original_stdint = dup(STDIN_FILENO);
	if (ft_strcmp(head->tokens->value[0], "exit") == 0)
		exit(1);
	multi_commands(head);
	dup2(original_stdint, STDIN_FILENO);
}
