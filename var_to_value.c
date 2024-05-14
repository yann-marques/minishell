/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_to_value.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yanolive <yanolive@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 17:13:19 by yanolive          #+#    #+#             */
/*   Updated: 2024/05/13 17:14:42 by yanolive         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_if_replace(char *str, int i_var);
static int	replace_var_call(t_ms *head, char **str, int k, int i);

char	**var_to_value(char **tab, t_ms *head)
{
	int	i;
	int	k;

	k = 0;
	while (tab && tab[k])
	{
		i = 0;
		while (tab[k] && tab[k][i])
		{
			if (check_if_replace(tab[k], i))
			{
				if (!replace_var_call(head, tab, k, i) || !tab[k][0])
				{
					strtab_clear(tab);
					return (NULL);
				}
				if (tab[k] && ft_strlen_to(tab[k], '\0') < i)
					break ;
			}
			++i;
		}
		++k;
	}
	return (tab);
}

static int	check_if_replace(char *str, int i_var)
{
	int	i;
	int	replace;

	replace = 1;
	i = -1;
	if (str[i_var] != '$' || (str[i_var] == '$' && (str[i_var + 1] == ' '
				|| str[i_var + 1] == '\'' || str[i_var + 1] == '"'
				|| !str[i_var + 1])))
		return (0);
	while (str[++i] && i < i_var)
	{
		replace = 1;
		if (str[i] == '"' && quotes_jump(&str[i]) > i_var)
			break ;
		if (str[i] == '\'' && ft_strchr(&str[i + 1], '\'') && ++i)
		{
			replace = 0;
			while (str[i] && str[i] != '\'' && i != i_var)
				++i;
		}
		if (!str[i])
			break ;
	}
	return (replace);
}

static char	*find_var(char *str, int i);

static int	replace_var_call(t_ms *head, char **str, int k, int i)
{
	char	*var;
	char	*begin;
	char	*end;

	begin = find_var(str[k], i);
	if (!begin)
		return (0);
	var = get_var_value(head, begin);
	end = ft_strndup(&str[k][i + ft_strlen_to(begin, '\0') + 1], 0);
	free(begin);
	if (!end)
		return (0);
	if (i == 0)
		i = -1;
	var = ms_join_three(ft_strndup(str[k], i), var, end, 0);
	if (!var)
		return (0);
	free(str[k]);
	str[k] = var;
	return (1);
}

static char	*find_var(char *str, int i)
{
	char	*var;
	int		tmp;

	tmp = ft_strlen_to(&str[i + 1], ' ');
	if (tmp > ft_strlen_to(&str[i + 1], '"'))
		tmp = ft_strlen_to(&str[i + 1], '"');
	if (tmp > ft_strlen_to(&str[i + 1], '\''))
		tmp = ft_strlen_to(&str[i + 1], '\'');
	if (str[i + 1] == '?')
		tmp = 1;
	var = ft_strndup(&str[i + 1], tmp);
	if (!var)
		return (NULL);
	return (var);
}
