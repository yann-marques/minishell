#include "../minishell.h"
#include "../gnl/get_next_line.h"

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
	if (!path_doc)
	{
		perror("Error joining the tmp directory");
		exit(EXIT_FAILURE);
	}
	if (access(path_doc, F_OK) == 0)
		return (get_random_tmp_path());
	return (path_doc);
}

int	do_needed_files(t_ms *head)
{
	int		outfile;
	t_token	*tmp;

	tmp = head->tokens;
	while (tmp)
	{
		if (tmp->type == _redirection)
		{
			if (tmp->value[0][0] == '>' && access(tmp->value[1], F_OK) != 0)
			{
				outfile = open(tmp->value[1], O_CREAT | O_WRONLY | O_TRUNC, 0644);
				if (!outfile)
					return (-1);
				close(outfile);
			}
		}
		if (tmp->type == _append)
		{
			if (access(tmp->value[1], F_OK) != 0)
			{
				outfile = open(tmp->value[1], O_CREAT | O_WRONLY | O_APPEND, 0644);
				if (!outfile)
					perror_exit(" ", -1);
				close(outfile);
			}
		}
		tmp = tmp->next;
	}
	return (1);
}