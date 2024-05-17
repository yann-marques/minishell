/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymarques <ymarques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 11:39:31 by ymarques          #+#    #+#             */
/*   Updated: 2024/05/17 15:29:48 by ymarques         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"
#include "../../gnl/get_next_line.h"

static int	free_rest_gnl(int fd, char *line, char *limiter, int return_code)
{
	if (line)
		free(line);
	free(limiter);
	close(fd);
	line = get_next_line(fd);
	free(line);
	return (return_code);
}

int	heredoc_addback(t_heredoc **here_doc, char *line)
{
	t_heredoc	*tmp;
	t_heredoc	*new;

	new = malloc(sizeof(t_heredoc));
	if (!new)
		return (-1);
	new->line = line;
	new->next = NULL;
	tmp = *here_doc;
	while (tmp && tmp->next)
		tmp = tmp->next;
	if (!*here_doc)
		*here_doc = new;
	else
		tmp->next = new;
	return (0);
}

void	expand_if_no_double_quote(char *lim, char **tab, t_ms *head, int *k)
{
	char	*line_without_dq;
	int		i;

	*k = -1;
	i = 0;
	if (isin_dblquotes(lim, 1))
	{
		tab = var_to_value(tab, head);
		if (!(*tab))
			error_exit(" Error to expands values in tab", EXIT_FAILURE);
		while (tab[i])
		{
			line_without_dq = ft_strdup_noquotes(tab[i]);
			if (!line_without_dq)
				error_exit(" Error: removing double quotes on line", 1);
			free(tab[i]);
			tab[i] = line_without_dq;
			i++;
		}
	}
}

void	fill_heredoc(char *line, char *lim, t_ms *head, int fd)
{
	t_heredoc	*heredoc;
	char		**tab;
	char		*lim_nq;
	int			k;

	heredoc = NULL;
	lim_nq = ft_strdup_noquotes(lim);
	if (!lim_nq)
		error_exit(" Error: removing double quotes on limiter", EXIT_FAILURE);
	while (line && ft_strncmp(line, lim_nq, ft_strlen(lim_nq) + 1) != 0)
	{
		write(STDOUT_FILENO, ">", 1);
		heredoc_addback(&heredoc, line);
		line = get_next_line(STDIN_FILENO);
	}
	free(lim_nq);
	if (!heredoc)
		return ;
	tab = t_heredoc_to_strtab(heredoc);
	if (!tab)
		error_exit(" Error to transform heredoc list into tab", EXIT_FAILURE);
	expand_if_no_double_quote(lim, tab, head, &k);
	while (tab[++k])
		write(fd, tab[k], ft_strlen(tab[k]));
	strtab_clear(tab);
}

char	*here_doc(t_ms *head, t_token *token)
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
	{
		free_rest_gnl(tmp_fd, line, limiter, EXIT_FAILURE);
		return (path_doc);
	}
	fill_heredoc(line, limiter, head, tmp_fd);
	free_rest_gnl(tmp_fd, NULL, limiter, 0);
	return (path_doc);
}
