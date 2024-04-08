#include "minishell.h"
#include "gnl/get_next_line.h"

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

int is_builtin(t_ms *head, t_token *token)
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

int    pipe_and_exec(t_ms *head, t_token *token, int last_command)
{
	int		pid;
	int		tmp_fd;
	int		fd[2];

	if (is_builtin(head, token) != 1)
		return (0);
	if (pipe(fd) == -1)
		error_exit("Error with the pipe");
	pid = fork();
	if (pid == -1)
		error_exit("Error with the pipe");
	if (pid == 0)
	{
		if (access("/tmp/ms_tmp", F_OK) == 0)
		{
			tmp_fd = open("/tmp/ms_tmp", O_RDONLY, 0644);
			if (tmp_fd == -1)
				error_exit("Error with ms_tmp");
			dup2(tmp_fd, STDIN_FILENO);
			unlink("/tmp/ms_tmp");
			close(tmp_fd);
		}
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
		if (!last_command)
			dup2(fd[0], STDIN_FILENO);
		close(fd[1]);
		close(fd[0]);
	}
	return (pid);
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

int	get_nb_cmd(t_token *token)
{
	t_token *tmp;
	int	i;

	i = 0;
	tmp = token;
	while(tmp)
	{
		if (tmp->type == _cmd_grp)
			i++;
		tmp = tmp->next;
	}
	return (i);
}


void	creat_needed_files(t_token *tokens)
{
	int		outfile;
	t_token *tmp;

	tmp = tokens;
	while (tmp)
	{
		if (tmp->type == _redirection)
		{
			if (tmp->value[0][0] == '>')
			{
				if (access(tmp->next->value[0], F_OK) != 0)
				{
					outfile = open(tmp->next->value[0], O_CREAT | O_WRONLY | O_TRUNC, 0644);
					if (!outfile)
						error_exit("Error for creating output file");
				}
			}
		}
		if (tmp->type == _append)
		{
			if (access(tmp->next->value[0], F_OK) != 0)
			{
				outfile = open(tmp->next->value[0], O_CREAT | O_WRONLY | O_APPEND, 0644);
				if (!outfile)
					error_exit("Error for creating output file");
			}
		}
		tmp = tmp->next;
	}
}

int	redirection_out(t_ms *head, t_token *token)
{
	int		outfile;
	char 	buffer[4096];
    ssize_t bytes_read;
	int		pid;

	pid = 0;
	if (token->next->type == _redirection)
		outfile = open(token->next->value[1], O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (token->next->type == _append)
		outfile = open(token->next->value[1], O_CREAT | O_WRONLY | O_APPEND, 0644);
	if (!outfile)
		error_exit("Error with the outfile");
	pid = pipe_and_exec(head, token, 0);
	while ((bytes_read = read(STDIN_FILENO, buffer, sizeof(buffer))) > 0) {
		write(outfile, buffer, bytes_read);
	}
	close(outfile);
	return (pid); 
}

void	redirection_in(t_token *token)
{
	int	infile;

	if (token->value[1])
		infile = open(token->value[1], O_RDONLY, 0644);
	if (!infile)
		error_exit("Error with infile");
	dup2(infile, STDIN_FILENO);
	close(infile);
}

void	free_rest_gnl(int fd, char *line, char *limiter)
{
	free(line);
	free(limiter);
	close(fd);
	line = get_next_line(fd);
	free(line);
}

void	here_doc(t_ms *head, t_token *token)
{
	char	*line;
	int		tmp_fd;
	char	*limiter;

	(void) head;
	limiter = ft_strjoin(token->value[1], "\n");
	tmp_fd = open("/tmp/ms_tmp", O_CREAT | O_TRUNC | O_WRONLY | O_APPEND, 0644);
	if (tmp_fd == -1)
		error_exit("Error with fileout");
	write(STDOUT_FILENO, ">", 1);
	line = get_next_line(0);
	if (!line)
		return (free_rest_gnl(tmp_fd, line, limiter));
	while (ft_strncmp(line, limiter, ft_strlen(limiter) + 1) != 0)
	{
		write(STDOUT_FILENO, ">", 1);
		write(tmp_fd, line, ft_strlen(line));
		free(line);
		line = get_next_line(0);
		if (!line)
			break ;
	}
	free_rest_gnl(tmp_fd, line, limiter);
	close(tmp_fd);
}

int multi_commands(t_ms *head)
{
    t_token *token;

	creat_needed_files(head->tokens);
    token = get_n_token(head->tokens, head->token_count);
	if (token->type == _redirection && token->value[0][0] == '<')
	{
		redirection_in(token);
		head->token_count += 1;
		token = token->next;
	}
	if (token->type == _delimiter && token->value[1])
	{
		here_doc(head, token);
		head->token_count += 1;
		token = token->next;
	}
	if (token->type == _cmd_grp && token->next && token->next->type == _delimiter && token->next->value[1] && token->next->next && token->next->next->type == _pipe)
	{
		here_doc(head, token->next);
		pids_addback(head->pids, pipe_and_exec(head, token, 0));
		head->token_count += 3;
		token = token->next->next->next;
	}
	if (token->type == _cmd_grp && token->next && token->next->type == _delimiter && token->next->value[1] && !token->next->next)
	{
		here_doc(head, token->next);
		pids_addback(head->pids, pipe_and_exec(head, token, 1));
		return (0);
	}
    while (token)
    {
        if (token->type == _cmd_grp && token->next && token->next->type == _pipe && token->next->next && token->next->next->type == _cmd_grp)
            pids_addback(head->pids, pipe_and_exec(head, token, 0));
		else if (token->type == _cmd_grp && token->next && token->next->type == _redirection && token->next->value[0][0] == '>' && token->next->value[1])
			pids_addback(head->pids, redirection_out(head, token));
		else if (token->type == _cmd_grp && token->next && token->next->type == _append && token->next->value[1])
			pids_addback(head->pids, redirection_out(head, token));
		else if (token->type == _cmd_grp)
			pids_addback(head->pids, pipe_and_exec(head, token, 1));
		head->token_count += 1;
        token = token->next;
    }
	while(head->pids)
	{
		waitpid(head->pids->pid, NULL, 0);
		head->pids = head->pids->next;
	}
    return (0);
}

void	command_manager(t_ms *head)
{
	int original_stdint;
	
	if (ft_strcmp(head->tokens->value[0], "exit") == 0)
		ms_exit(head);
	original_stdint = dup(STDIN_FILENO);
	multi_commands(head);
	dup2(original_stdint, STDIN_FILENO);
}
