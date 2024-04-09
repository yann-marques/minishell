#include "../minishell.h"
#include "../gnl/get_next_line.h"

char	*get_random_tmp_path(t_ms *head)
{
	int				fd;
	ssize_t			num_bytes_read;
	unsigned char	buffer[16];
	char			*path_doc;

	fd = open("/dev/random", O_RDONLY);
	if (fd == -1)
		error_exit(head, EXIT_FAILURE, NULL);
	num_bytes_read = read(fd, buffer, 16);
	if (num_bytes_read == -1)
	{
		close(fd);
		error_exit(head, EXIT_FAILURE, NULL);
	}
	close(fd);
	path_doc = ft_strjoin("/tmp/ms_heredoc_", (char *) buffer);
	printf("Path: %s", path_doc);
	if (!path_doc)
		error_exit(head, EXIT_FAILURE, NULL);
	if (access(path_doc, F_OK) == 0)
		return (get_random_tmp_path(head));
	return (path_doc);
}

void	creat_needed_files(t_ms *head, t_token *tokens)
{
	int		outfile;
	t_token	*tmp;

	tmp = tokens;
	while (tmp)
	{
		if (tmp->type == _redirection)
		{
			if (tmp->value[0][0] == '>')
			{
				if (access(tmp->value[1], F_OK) != 0)
				{
					outfile = open(tmp->value[1], O_CREAT | O_WRONLY | O_TRUNC, 0644);
					if (!outfile)
						error_exit(head, EXIT_FAILURE, NULL);
				}
			}
		}
		if (tmp->type == _append)
		{
			if (access(tmp->value[1], F_OK) != 0)
			{
				outfile = open(tmp->value[1], O_CREAT | O_WRONLY | O_APPEND, 0644);
				if (!outfile)
					error_exit(head, EXIT_FAILURE, NULL);
			}
		}
		tmp = tmp->next;
	}
}