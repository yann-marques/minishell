#include "../../minishell.h"
#include "../../gnl/get_next_line.h"

static int	free_rest_gnl(int fd, char *line, char *limiter, int return_code)
{
	free(line);
	free(limiter);
	close(fd);
	line = get_next_line(fd);
	free(line);
	return (return_code);
}

char	*here_doc(t_token *token)
{
	char	*line;
	char	*path_doc;
	int		tmp_fd;
	char	*limiter;

	limiter = ft_strjoin(token->value[1], "\n");
	path_doc = get_random_tmp_path();
	tmp_fd = open(path_doc, O_CREAT | O_TRUNC | O_WRONLY | O_APPEND, 0644);
	if (tmp_fd == -1)
		error_exit("Error with fileout", -1);
	write(STDOUT_FILENO, ">", 1);
	line = get_next_line(0);
	if (!line)
		exit(free_rest_gnl(tmp_fd, line, limiter, EXIT_FAILURE));
	while (ft_strncmp(line, limiter, ft_strlen(limiter) + 1) != 0)
	{
		write(STDOUT_FILENO, ">", 1);
		write(tmp_fd, line, ft_strlen(line));
		free(line);
		line = get_next_line(0);
		if (!line)
			break ;
	}
	free_rest_gnl(tmp_fd, line, limiter, 0);
	return (path_doc);
}
