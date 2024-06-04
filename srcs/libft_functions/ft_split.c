/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yanolive <yanolive@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 13:06:20 by ymarques          #+#    #+#             */
/*   Updated: 2024/06/04 15:01:43 by yanolive         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	count_words(char const *str, char c)
{
	size_t	i;
	int		count;

	i = 0;
	count = 0;
	if (str[0] == '\0')
		return (0);
	while (str[i] == c)
		i++;
	if (i == ft_strlen(str))
		return (0);
	i = 0;
	while (str[i] != '\0')
	{
		if (ft_isprint(str[i]) && str[i] != c
			&& (str[i + 1] == c || str[i + 1] == '\0'))
			count++;
		i++;
	}
	if (count == 0)
		return (0);
	return (count);
}

static void	fill_var(int *i, int *j, int *k, int *l)
{
	if (i)
		*i = -1;
	if (j)
		*j = 0;
	if (k)
		*k = 0;
	if (l)
		*l = -1;
}

char	**ft_split(char const *s, char c)
{
	int		i;
	int		j;
	int		k;
	int		l;
	char	**new_s;

	if (!s)
		return (NULL);
	new_s = (char **) ft_calloc(sizeof(char *), count_words(s, c) + 1);
	if (!new_s)
		return (NULL);
	fill_var(&i, &j, &k, &l);
	while (++i < count_words(s, c))
	{
		while (s[j] == c)
			j++;
		while (s[j] != '\0' && s[j] != c && ++k)
			j++;
		new_s[i] = (char *) ft_calloc(sizeof(char), k + 1);
		while (++l < k)
			new_s[i][l] = s[j - k + l];
		fill_var(NULL, NULL, &k, &l);
	}
	return (new_s);
}
