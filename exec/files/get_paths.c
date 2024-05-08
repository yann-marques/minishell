/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_paths.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymarques <ymarques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 11:39:31 by ymarques          #+#    #+#             */
/*   Updated: 2024/05/08 16:06:48 by ymarques         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"
#include "../../gnl/get_next_line.h"

static const char	*fill_buffer(char *buffer)
{
	const char	*g_table;
	int			i;

	i = 0;
	g_table = "abcdefghijklmnopqrstuvwxyz0123456789_";
	while (i < 16)
	{
		buffer[i] = g_table[buffer[i] % 37];
		i++;
	}
	return (buffer);
}

char	*get_random_tmp_path(void)
{
	int			fd;
	char		buffer[16];
	const char	*buffer_table;
	char		*path_doc;

	fd = open("/dev/random", O_RDONLY);
	if (fd == -1)
		perror_exit("Error opening /dev/random", -1);
	if (read(fd, buffer, 16) == -1)
	{
		close(fd);
		perror_exit("Error reading from /dev/random", fd);
	}
	buffer_table = fill_buffer((char *)buffer);
	close(fd);
	path_doc = ft_strjoin("/tmp/ms_heredoc_", buffer_table);
	if (!path_doc)
		error_exit("Error joining the tmp directory", -1);
	return (path_doc);
}

char	*make_command_path(char	*path, t_token *token)
{
	char	*path_slash;
	char	*path_command;

	path_slash = ft_strjoin(path, "/");
	if (!path_slash)
		error_exit("", -1);
	path_command = ft_strjoin(path_slash, token->value[0]);
	if (!path_command)
		error_exit("", -1);
	free(path_slash);
	return (path_command);
}

char	*find_path(t_ms *head, t_token *token)
{
	char	**paths;
	char	*path;
	char	*path_command;
	int		i;

	if (access(token->value[0], F_OK) != -1)
		return (token->value[0]);
	path = get_var_value(head, "PATH");
	if (!path)
		return (NULL);
	paths = ft_split(path, ':');
	i = 0;
	while (paths[i])
	{
		path_command = make_command_path(paths[i++], token);
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
