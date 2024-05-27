/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymarques <ymarques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 11:39:31 by ymarques          #+#    #+#             */
/*   Updated: 2024/05/27 14:58:22 by ymarques         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"
#include "../../../gnl/get_next_line.h"

static void	handle_sigint_heredoc(int signum)
{
	g_sig_received = signum;
	close(STDIN_FILENO);
}

static int	get_first_line(char **line, char **limiter, int *tmp_fd)
{
	signal(SIGINT, handle_sigint_heredoc);
	write(STDOUT_FILENO, "> ", 2);
	*line = get_next_line(STDIN_FILENO);
	if (!(*line))
	{
		free_rest_gnl(*tmp_fd, *line, *limiter, EXIT_FAILURE);
		return (0);
	}
	return (1);
}

char	*here_doc(t_ms *head, t_token *token)
{
	char	*line;
	char	*path_doc;
	int		tmp_fd;
	char	*limiter;
	int		pid;

	limiter = ft_strjoin(token->value[1], "\n");
	path_doc = get_random_tmp_path(head);
	tmp_fd = open(path_doc, O_CREAT | O_TRUNC | O_WRONLY | O_APPEND, 0644);
	if (tmp_fd == -1)
		error_exit(head, "Error with fileout", -1);
	pid = fork();
	if (pid != -1 && pid == 0)
	{
		if (!get_first_line(&line, &limiter, &tmp_fd))
			exit(EXIT_FAILURE);
		fill_heredoc(line, limiter, head, tmp_fd);
		free_rest_gnl(tmp_fd, NULL, limiter, 0);
		if (g_sig_received == SIGINT)
			unlink(path_doc);
		exit(EXIT_SUCCESS);
	}
	else
		waitpid(pid, NULL, 0);
	return (path_doc);
}
