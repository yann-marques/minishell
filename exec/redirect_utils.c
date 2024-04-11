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
