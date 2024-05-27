/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymarques <ymarques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 17:09:46 by yanolive          #+#    #+#             */
/*   Updated: 2024/05/20 14:47:15 by ymarques         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "gnl/get_next_line.h"

void	perror_exit(t_ms *head, char *str, int status)
{
	if (str)
	{
		perror(str);
		exit_free_head(head, 0);
		if (status != -1)
			exit(status);
		else
			exit(EXIT_FAILURE);
	}
	perror("");
	exit_free_head(head, 0);
	if (status != -1)
		exit(status);
	else
		exit(EXIT_FAILURE);
}

void	error_exit(t_ms *head, char *str, int status)
{
	char	*error_str;

	if (str)
	{
		error_str = ft_strjoin(str, "\n");
		if (!error_str)
		{
			exit_free_head(head, 0);
			exit(EXIT_FAILURE);
		}
		write(2, error_str, ft_strlen(error_str));
		free(error_str);
		exit_free_head(head, 0);
		if (status != -1)
			exit(status);
		else
			exit(EXIT_FAILURE);
	}
	exit_free_head(head, 0);
	if (status != -1)
		exit(status);
	else
		exit(EXIT_FAILURE);
}
