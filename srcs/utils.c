/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yanolive <yanolive@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 17:12:59 by yanolive          #+#    #+#             */
/*   Updated: 2024/06/06 15:37:22 by yanolive         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strndup(char *str, int n)
{
	char	*dst;
	int		i;

	if (n == -1)
		n = 0;
	else if (n == 0)
		n = ft_strlen_to(str, '\0');
	dst = malloc(sizeof(char) * (n + 1));
	if (!dst)
		return (NULL);
	i = 0;
	while (str[i] && i < n)
	{
		dst[i] = str[i];
		++i;
	}
	dst[i] = '\0';
	return (dst);
}

int	ft_strtab_len(char **tab)
{
	int	k;

	k = 0;
	while (tab && tab[k])
		++k;
	return (k);
}

int	ft_strlen_to(char *str, char c)
{
	int	i;

	i = 0;
	while (str && str[i] && str[i] != c)
		++i;
	return (i);
}

int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1[i] || s2[i])
	{
		if (s1[i] != s2[i])
			return (s1[i] - s2[i]);
		++i;
	}
	if (s1[i] != s2[i])
		return (s1[i] - s2[i]);
	return (0);
}

int	quotes_jump(char *str)
{
	int		i;
	char	c;

	if ((str[0] != '\'' && str[0] != '"') || !ft_strchr(&str[1], str[0]))
		return (0);
	c = str[0];
	i = 1;
	while (str[i] && str[i] != c)
		++i;
	return (i);
}
