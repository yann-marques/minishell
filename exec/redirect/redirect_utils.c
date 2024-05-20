/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymarques <ymarques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 11:39:31 by ymarques          #+#    #+#             */
/*   Updated: 2024/05/20 14:53:58 by ymarques         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"
#include "../../gnl/get_next_line.h"

void	rd_null(t_ms *head)
{
	int	fd_null;

	fd_null = open("/dev/null", O_RDONLY, 0644);
	if (fd_null == -1)
		perror_exit(head, " ", EXIT_FAILURE);
	dup2(fd_null, STDIN_FILENO);
	close(fd_null);
}

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

int	redirection_in(t_ms *head, t_token *token)
{
	int	infile;

	if (!token->value[1])
		return (-1);
	infile = open(token->value[1], O_RDONLY, 0644);
	if (infile == -1)
	{
		rd_null(head);
		return (-1);
	}
	dup2(infile, STDIN_FILENO);
	close(infile);
	return (1);
}
