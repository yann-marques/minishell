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

t_token	*get_next_pipe(t_token *tk)
{
	t_token *tmp;

	tmp = tk;
	if (tmp->type == _pipe && tmp->next)
		return (tmp->next);
	while(tmp && tmp->type != _pipe)
		tmp = tmp->next;
	if (tmp && tmp->type == _pipe && tmp->next)
		return (tmp->next);
	return (tmp);
}

int	do_needed_files(t_token *tk)
{
	int		outfile;
	t_token	*tmp;

	tmp = tk;
	while (tmp)
	{
		if (tmp->type == _redirection && tmp->value[0][0] == '>' && !tmp->value[0][1])
		{
			outfile = open(tmp->value[1], O_CREAT | O_WRONLY | O_TRUNC, 0644);
			if (outfile != -1)
				close(outfile);
			else
			{
				tmp = get_next_pipe(tmp);
				continue ;
			}
		}
		if (tmp->type == _redirection && tmp->value[0][0] == '<' && !tmp->value[0][1])
		{
			outfile = open(tmp->value[1], O_RDONLY, 0644);
			if (outfile != -1)
				close(outfile);
			else
			{
				tmp = get_next_pipe(tmp);
				continue ;
			}
		}
		if (tmp->type == _append)
		{
			outfile = open(tmp->value[1], O_CREAT | O_WRONLY | O_APPEND, 0644);
			if (outfile != -1)
				close(outfile);
			else
			{
				tmp = get_next_pipe(tmp);
				continue ;
			}
		}
		tmp = tmp->next;
	}
	return (1);
}

int	is_file_error_in_pipe(t_token *token)
{
	int		outfile;
	t_token	*tmp;

	tmp = token;
	if (tmp->type == _pipe && tmp->next)
		tmp = tmp->next;
	while (tmp)
	{
		if (tmp->type == _redirection && tmp->value[0][0] == '>' && !tmp->value[0][1])
		{
			outfile = open(tmp->value[1], O_CREAT | O_WRONLY | O_TRUNC, 0644);
			if (outfile != -1)
				close(outfile);
			else
				return (1);
		}
		if (tmp->type == _redirection && tmp->value[0][0] == '<' && !tmp->value[0][1])
		{
			outfile = open(tmp->value[1], O_RDONLY, 0644);
			if (outfile != -1)
				close(outfile);
			else
				return (1);
		}
		if (tmp->type == _append)
		{
			outfile = open(tmp->value[1], O_CREAT | O_WRONLY | O_APPEND, 0644);
			if (outfile != -1)
				close(outfile);
			else
				return (1);
		}
		if (tmp->type == _pipe)
			break ;
		tmp = tmp->next;
	}
	return (0);
}