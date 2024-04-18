#include "../minishell.h"
#include "../gnl/get_next_line.h"

int	redirection_out(t_ms *head, t_token *token)
{
	int		outfile;
	char	buffer[4096];
	ssize_t	bytes_read;
	int		pid;

	pid = 0;
	if (token->next->type == _redirection)
		outfile = open(token->next->value[1], O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (token->next->type == _append)
		outfile = open(token->next->value[1], O_CREAT | O_WRONLY | O_APPEND, 0644);
	if (!outfile)
		error_exit("Error with the outfile");
	pid = pipe_and_exec(head, token, NULL, 0);
	bytes_read = read(STDIN_FILENO, buffer, sizeof(buffer));
	while (bytes_read > 0)
	{
		write(outfile, buffer, bytes_read);
		bytes_read = read(STDIN_FILENO, buffer, sizeof(buffer));
	}
	close(outfile);
	return (pid);
}

void	redirection_in(char *path)
{
	int	infile;

	if (!path)
		error_exit("Error path redirection in NULL");
	infile = open(path, O_RDONLY, 0644);
	if (!infile)
		error_exit("Error with infile");
	dup2(infile, STDIN_FILENO);
	close(infile);
}

int	do_redirection_in(t_ms *head, t_token *tk)
{
	t_token	*tkn;
	t_token *tmp;
	t_token *tmp_prev;
	int		error;
	int		i;

	error = 0;
	if (tk->type == _redirection)
	{
		tmp = tk;
		i = 1;
		while (tmp && tmp->type == _redirection && tmp->value[0][0] == '<')
		{
			while (tmp->value[i])
				i++;
			if (i > 1)
				i--;
			if (access(tmp->value[i], R_OK) != 0)
			{
				redirection_in("/dev/null");
				perror_str(" ");
			}
			else
				redirection_in(tmp->value[i]);
			tmp_prev = tmp;
			tmp = tmp->next;
			head->token_count += 1;
			i = 1;
		}
		if (tmp && tmp->type == _pipe && tmp->next && tmp->next->type == _cmd_grp && tmp->next->next && tmp->next->next->type == _pipe)
			pids_addback(&head->pids, pipe_and_exec(head, tmp->next, NULL, 0));
		if (tmp && tmp->next && tmp->next->next)
			*tk = *tmp->next->next;
		else
			*tk = *tmp_prev;
		head->token_count += 1;
	}
	if (tk->type == _cmd_grp)
	{
		tkn = tk->next;
		tmp = tkn;
		i = 1;
		while (tmp && tmp->type == _redirection && tmp->value[0][0] == '<')
		{
			while (tmp->value[i])
				i++;
			if (i > 1)
				i--;
			if (access(tmp->value[i], R_OK) != 0)
			{
				error = 1;
				redirection_in("/dev/null");
				perror_str(" ");
			}
			else
				redirection_in(tmp->value[i]);
			tmp_prev = tmp;
			tmp = tmp->next;
			head->token_count += 1;
			i = 1;
		}
		if (tmp && tmp->type == _pipe && tmp->next && tmp->next->type == _cmd_grp)
			pids_addback(&head->pids, pipe_and_exec(head, tk, NULL, 0));
		else
		{
			if (error)
				head->last_status = EXIT_FAILURE;
			else
				pids_addback(&head->pids, pipe_and_exec(head, tk, NULL, 1));
		}
		if (tmp)
			*tk = *tmp;
		else
			*tk = *tmp_prev;
		head->token_count += 1;
	}
	return (1);
}

static void	free_rest_gnl(int fd, char *line, char *limiter)
{
	free(line);
	free(limiter);
	close(fd);
	line = get_next_line(fd);
	free(line);
}

char	*here_doc(t_ms *head, t_token *token)
{
	char	*line;
	char	*path_doc;
	int		tmp_fd;
	char	*limiter;

	(void) head;
	limiter = ft_strjoin(token->value[1], "\n");
	path_doc = get_random_tmp_path();
	tmp_fd = open(path_doc, O_CREAT | O_TRUNC | O_WRONLY | O_APPEND, 0644);
	if (tmp_fd == -1)
		error_exit("Error with fileout");
	write(STDOUT_FILENO, ">", 1);
	line = get_next_line(0);
	if (!line)
	{
		free_rest_gnl(tmp_fd, line, limiter);
		exit(EXIT_FAILURE);
	}
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
	return (path_doc);
}
