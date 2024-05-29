/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yanolive <yanolive@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 11:39:31 by ymarques          #+#    #+#             */
/*   Updated: 2024/05/29 14:23:46 by yanolive         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"
#include "../../../gnl/get_next_line.h"

int	free_rest_gnl(int fd, char *line, char *limiter, int return_code)
{
	if (!line)
		write (1, "\n", 1);
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
	if (!ft_strchr(lim, '\'') && !ft_strchr(lim, '"'))
	{
		tab = var_to_value(tab, head);
		if (!(*tab))
			error_exit(head, " Error to expands values in tab", EXIT_FAILURE);
		while (tab[i])
		{
			line_without_dq = ft_strdup_noquotes(tab[i]);
			if (!line_without_dq)
				error_exit(head, " Error: removing double quotes on line", 1);
			free(tab[i]);
			tab[i] = line_without_dq;
			i++;
		}
	}
}

void	make_tab(t_ms *head, t_heredoc *heredoc, char *lim, int fd)
{
	char		**tab;
	int			k;
	
	k = 0;
	tab = t_heredoc_to_strtab(heredoc);
	if (!tab)
		error_exit(head, " Error to transform heredoc list", EXIT_FAILURE);
	expand_if_no_double_quote(lim, tab, head, &k);
	while (tab[++k])
		write(fd, tab[k], ft_strlen(tab[k]));
	strtab_clear(tab);
}

void	fill_heredoc(char *line, char *lim, t_ms *head, int fd)
{
	t_heredoc		*heredoc;
	char			*lim_nq;
	char			*line_back;

	heredoc = NULL;
	lim_nq = ft_strdup_noquotes(lim);
	if (!lim_nq)
		error_exit(head, " Error: removing double quotes", EXIT_FAILURE);
	while (line && ft_strncmp(line, lim_nq, ft_strlen(lim_nq) + 1) != 0)
	{
		line_back = ft_strjoin(line, "\n");
		free(line);
		if (!line_back)
			return ;
		heredoc_addback(&heredoc, line_back);
		line = readline("> ");
	}
	free(line);
	free(lim_nq);
	if (!heredoc)
		return ;
	make_tab(head, heredoc, lim, fd);
}
