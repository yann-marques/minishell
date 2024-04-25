#include "../minishell.h"
#include "../gnl/get_next_line.h"

int	check_file_out(t_token *token)
{
	int		outfile;

	if (token->type == _redirection)
		outfile = open(token->value[1], O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (token->type == _append)
		outfile = open(token->value[1], O_CREAT | O_WRONLY | O_APPEND, 0644);
	if (outfile == -1)
		return (-1);
	return (outfile);
}

void	redirection_out(int fd_out)
{
	char	buffer[4096];
	ssize_t	bytes_read;

	bytes_read = read(STDIN_FILENO, buffer, sizeof(buffer));
	while (bytes_read > 0)
	{
		write(fd_out, buffer, bytes_read);
		bytes_read = read(STDIN_FILENO, buffer, sizeof(buffer));
	}
	close(fd_out);
}

int	redirection_in(t_token *token)
{
	int	infile;

	if (!token->value[1])
		return (-1);
	infile = open(token->value[1], O_RDONLY, 0644);
	if (infile == -1)
	{
		infile = open("/dev/null", O_RDONLY, 0644);
		if (infile == -1)
			perror_exit(" ", EXIT_FAILURE);
		dup2(infile, STDIN_FILENO);
		return (-1);
	}
	dup2(infile, STDIN_FILENO);
	close(infile);
	return (1);
}

int	do_redirection_out(t_ms *head, t_token *tk)
{
	t_token *tmp;
	int		fd_out;

	(void) head;
	tmp = tk;
	fd_out = -1;
	while(tmp)
	{
		if ((tmp->type == _redirection && tmp->value[0][0] == '>') || tmp->type == _append)
		{
			fd_out = check_file_out(tmp);
			if (fd_out == -1)
				return (perror_str(" ", -42));
		}
		else
			break ;
		tmp = tmp->next;
	}
	if (fd_out != -1)
		redirection_out(fd_out);
	return (1);
}

int	do_redirection_in(t_ms *head, t_token *tk)
{
	t_token *tmp;

	(void) head;
	tmp = tk;
	while(tmp)
	{
		if (tmp->type == _redirection && tmp->value[0][0] == '<')
		{
			if (redirection_in(tmp) == -1)
				return (perror_str(" ", -42));
		}
		else
			break ;
		tmp = tmp->next;
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
		error_exit("Error with fileout", -1);
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
