/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymarques <ymarques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 11:39:31 by ymarques          #+#    #+#             */
/*   Updated: 2024/05/29 13:11:06 by ymarques         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"
#include "../../../gnl/get_next_line.h"

static void	handle_sigint_heredoc(int signum)
{
	g_sig_received = signum;
	close(STDIN_FILENO);
}

static int	get_first_line(char **path_doc, char **line, int *tmp_fd)
{
	signal(SIGINT, handle_sigint_heredoc);
	signal(SIGQUIT, SIG_IGN);
	*line = readline("> ");
	if (!(*line))
	{
		free_rest_gnl(*tmp_fd, *line, *path_doc, EXIT_FAILURE);
		return (0);
	}
	return (1);
}

char	*here_doc(t_ms *head, t_token *token)
{
	char	*line;
	char	*path_doc;
	int		tmp_fd;
	int		pid;

	path_doc = get_random_tmp_path(head);
	tmp_fd = open(path_doc, O_CREAT | O_TRUNC | O_WRONLY | O_APPEND, 0644);
	if (tmp_fd == -1)
		error_exit(head, "Error with fileout", -1);
	pid = fork();
	signal(SIGQUIT, SIG_IGN);
	if (pid != -1 && pid == 0)
	{
		if (!get_first_line(&path_doc, &line, &tmp_fd))
			exit_free_head(head, 1);
		fill_heredoc(line, token->value[1], head, tmp_fd);
		close(tmp_fd);
		if (g_sig_received == SIGINT)
			unlink(path_doc);
		free(path_doc);
		exit_free_head(head, 1);
	}
	else
		waitpid(pid, NULL, 0);
	close(tmp_fd);
	return (path_doc);
}
