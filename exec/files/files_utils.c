/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   files_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymarques <ymarques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 11:39:31 by ymarques          #+#    #+#             */
/*   Updated: 2024/05/08 16:06:53 by ymarques         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"
#include "../../gnl/get_next_line.h"

int	is_file(char *name)
{
	t_dir	*directory;

	directory = opendir(name);
	if (directory != NULL)
	{
		closedir(directory);
		return (0);
	}
	if (errno == ENOTDIR)
		return (1);
	return (-1);
}

int	do_needed_files(t_token *tk)
{
	int		outfile;
	t_token	*tmp;

	tmp = tk;
	while (tmp)
	{
		if (tmp->type == _redirection || tmp->type == _append)
		{
			if (tmp->value[0][0] == '>' && !tmp->value[0][1])
				outfile = open(tmp->value[1], 0100 | 01 | 01000, 0644);
			if (tmp->value[0][0] == '<' && !tmp->value[0][1])
				outfile = open(tmp->value[1], 00, 0644);
			if (tmp->type == _append)
				outfile = open(tmp->value[1], 0100 | 01 | 02000, 0644);
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
		if (tmp->type == _redirection || tmp->type == _append)
		{
			if (tmp->value[0][0] == '>' && !tmp->value[0][1])
				outfile = open(tmp->value[1], 0100 | 01 | 01000, 0644);
			if (tmp->value[0][0] == '<' && !tmp->value[0][1])
				outfile = open(tmp->value[1], 00, 0644);
			if (tmp->type == _append)
				outfile = open(tmp->value[1], 0100 | 01 | 02000, 0644);
			if (outfile == -1)
				return (1);
			close(outfile);
		}
		if (tmp->type == _pipe)
			break ;
		tmp = tmp->next;
	}
	return (0);
}
