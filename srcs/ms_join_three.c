/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_join_three.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yanolive <yanolive@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 17:12:42 by yanolive          #+#    #+#             */
/*   Updated: 2024/06/04 14:42:59 by yanolive         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*recreate_var(char *var, char *begin, int *free_var);

char	*ms_join_three(char *s1, char *s2, char *s3, int free_var)
{
	char	*dst;

	s2 = recreate_var(s2, s1, &free_var);
	dst = ft_calloc(sizeof(char), (1 + ft_strlen_to(s1, '\0')
				+ ft_strlen_to(s2, '\0') + ft_strlen_to(s3, '\0')));
	if (!dst || !s1 || (free_var && !s2) || !s3)
	{
		if (dst)
			free(dst);
		if (s1)
			free(s1);
		if (s2 && free_var)
			free(s2);
		if (s3)
			free(s3);
		return (NULL);
	}
	ft_strlcpy(dst, s1, ft_strlen(s1) + 1);
	free(s1);
	ft_strlcpy(&dst[ft_strlen(dst)], s2, ft_strlen(s2) + 1);
	if (s2 && free_var)
		free(s2);
	ft_strlcpy(&dst[ft_strlen(dst)], s3, ft_strlen(s3) + 1);
	free(s3);
	return (dst);
}

static char	**add_dblquotes(char **tab);
static char	*strtab_to_str(char **tab);

static char	*recreate_var(char *var, char *begin, int *free_var)
{
	char	*dst;
	char	**tab;

	if (!begin)
		return (NULL);
	if (!isin_dblquotes(begin, ft_strlen(begin), 0)
		|| (var && !ft_strchr(var, ' ')))
	{
		if (free_var)
			return (var);
		return (ft_strdup(var));
	}
	*free_var = 1;
	if (!var)
		return (ft_strdup(""));
	tab = ft_split(var, ' ');
	tab = add_dblquotes(tab);
	dst = strtab_to_str(tab);
	if (!dst)
		return (NULL);
	strtab_clear(tab);
	return (dst);
}

int	isin_dblquotes(char *str, int i_target, bool dblquotes)
{
	int	i;
	int	replace;

	replace = 1;
	i = 0;
	while (str[i] && i < i_target)
	{
		replace = 1;
		if (str[i] == '"' || str[i] == '\'')
		{
			if (str[i] == '\'' || !dblquotes)
				replace = 0;
			i += quotes_jump(&str[i]);
		}
		if (i > i_target || !str[i])
			break ;
		++i;
	}
	return (replace);
}

static char	**add_dblquotes(char **tab)
{
	char	*tmp;
	int		k;

	if (!tab)
		return (NULL);
	k = 0;
	while (tab[k])
	{
		tmp = ms_join_three(ft_strdup("\""), tab[k], ft_strdup("\" "), 0);
		if (!tmp)
		{
			strtab_clear(tab);
			return (NULL);
		}
		if (!tab[k + 1])
			tmp[ft_strlen(tmp) - 1] = '\0';
		free(tab[k]);
		tab[k] = tmp;
		++k;
	}
	return (tab);
}

static char	*strtab_to_str(char **tab)
{
	char	*tmp;
	char	*dst;
	int		k;

	if (!tab)
		return (NULL);
	if (!tab[0])
		return (ft_strdup(""));
	if (!tab[1])
		return (ft_strdup(tab[0]));
	k = 0;
	tmp = tab[k];
	while (tab[++k])
	{
		dst = ft_strjoin(tmp, tab[k]);
		if (k > 1)
			free(tmp);
		if (!dst)
		{
			strtab_clear(tab);
			return (NULL);
		}
		tmp = dst;
	}
	return (dst);
}
