/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymarques <ymarques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 19:18:25 by ymarques          #+#    #+#             */
/*   Updated: 2024/04/05 12:04:36 by ymarques         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include "../minishell.h"

int	have_nl(char *str)
{
	int	i;

	i = 0;
	while (str && *str && str[i] != '\0')
	{
		if (str[i] == '\n')
			return (1);
		i++;
	}
	return (0);
}

char	*concat_buff(char *s1, char *s2)
{
	int		i;
	int		j;
	char	*new_s;

	i = -1;
	j = 0;
	if (!s1 || !s2)
		return (NULL);
	new_s = (char *) malloc((ft_strlen(s1) + ft_strlen(s2) + 1) * sizeof(char));
	if (!new_s)
		return (NULL);
	while (s1[++i] != '\0')
		new_s[i] = s1[i];
	while (s2[j] != '\0')
		new_s[i++] = s2[j++];
	free(s1);
	new_s[i] = '\0';
	return (new_s);
}

char	*restof_tmp(char *str)
{
	int		i;
	int		j;
	char	*restof_tmp;

	i = 0;
	j = 0;
	while (str[i] != '\n' && str[i] != '\0')
		i++;
	if (str[i] == '\0')
	{
		free(str);
		return (NULL);
	}
	restof_tmp = (char *) malloc((ft_strlen(str) - i + 1) * sizeof(char));
	if (!restof_tmp)
		return (NULL);
	while (str[++i] != '\0')
		restof_tmp[j++] = str[i];
	restof_tmp[j] = '\0';
	free(str);
	return (restof_tmp);
}
