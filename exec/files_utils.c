#include "../minishell.h"
#include "../gnl/get_next_line.h"

static void error_exit(char *str)
{
	printf("%s", str);
	exit(EXIT_FAILURE);
}

char	*get_random_tmp_path(void)
{
	int				fd;
	ssize_t			num_bytes_read;
	unsigned char	buffer[16];
	char			*path_doc;

	fd = open("/dev/random", O_RDONLY);
	if (fd == -1)
	{
		perror("Error opening /dev/random");
		exit(EXIT_FAILURE);
	}
	num_bytes_read = read(fd, buffer, 16);
	if (num_bytes_read == -1)
	{
		perror("Error reading from /dev/random");
		close(fd);
		exit(EXIT_FAILURE);
	}
	close(fd);
	path_doc = ft_strjoin("/tmp/ms_heredoc_", (char *) buffer);
	printf("Path: %s", path_doc);
	if (!path_doc)
	{
		perror("Error joining the tmp directory");
		exit(EXIT_FAILURE);
	}
	if (access(path_doc, F_OK) == 0)
		return (get_random_tmp_path());
	return (path_doc);
}

void	creat_needed_files(t_token *tokens)
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
						error_exit("Error for creating output file");
					close(outfile);
				}
			}
		}
		if (tmp->type == _append)
		{
			if (access(tmp->value[1], F_OK) != 0)
			{
				outfile = open(tmp->value[1], O_CREAT | O_WRONLY | O_APPEND, 0644);
				if (!outfile)
					error_exit("Error for creating output file");
				close(outfile);
			}
		}
		tmp = tmp->next;
	}
}