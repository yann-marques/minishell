#include "minishell.h"

char		**split_var(char **tab, int *k);
int			replace_var_call(t_ms *head, char **str, int k, int i);
int			check_if_replace(char *str, int i_var);
static char	*find_var(char *str, int i);
static char	*ms_join_three(char *s1, char *s2, char *s3);

char	**var_to_value(char **tab, t_ms *head)
{
	int		i;
	int		k;

	k = -1;
	while (tab && tab[++k])
	{
		i = -1;
		while (tab[k] && tab[k][++i])
		{
			if (check_if_replace(tab[k], i))
			{
				if (!replace_var_call(head, tab, k, i) || !tab[k][0])
				{
					strtab_clear(tab);
					return (NULL);
				}
				/* if ()
				{
					tmp = split_var(tab, &k);
					if (!tmp)
					{
						strtab_clear(tab);
						return (NULL);
					}
					free(tab);
					tab = tmp;
				} */
				if (tab[k] && ft_strlen_to(tab[k], '\0') < i)
					break ;
			}
		}
	}
	return (tab);
}

char	**split_var(char **tab, int *k)
{
	char	**new_tab;
	char	**split_dst;
	int		i;
	int		j;

	split_dst = ft_split(tab[*k], ' ');
	if (!split_dst)
		return (NULL);
	new_tab = malloc(sizeof(char *) * (ft_strtab_len(tab) + ft_strtab_len(split_dst)));
	if (!new_tab)
	{
		strtab_clear(split_dst);
		return (NULL);
	}
	i = -1;
	while (++i < *k)
		new_tab[i] = tab[i];
	j = -1;
	while (split_dst[++j] && !split_dst[j][0])
		free(split_dst[j]);
	i = -1;
	while (split_dst[++i + j])
		new_tab[i + *k] = split_dst[i + j];
	j = i + *k - 1;
	free(split_dst);
	free(tab[*k]);
	while (tab[*k + 1])
	{
		new_tab[i + *k] = tab[*k + 1];
		*k += 1;
	}
	new_tab[i + *k] = NULL;
	*k = j;
	return (new_tab);
}

int	replace_var_call(t_ms *head, char **str, int k, int i)
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
	var = ms_join_three(ft_strndup(str[k], i), var, end);
	if (!var)
		return (0);
	free(str[k]);
	str[k] = var;
	return (1);
}

int	check_if_replace(char *str, int i_var)
{
	int	i;
	int	replace;

	replace = 1;
	i = -1;
	if (str[i_var] != '$' || (str[i_var] == '$' && (str[i_var + 1] == ' '
		|| str[i_var + 1] == '\'' || str[i_var + 1] == '"' || !str[i_var + 1])))
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

static char	*ms_join_three(char *s1, char *s2, char *s3)
{
	char	*dst;

	dst = ft_calloc(sizeof(char), (1 + ft_strlen_to(s1, '\0')
				+ ft_strlen_to(s2, '\0') + ft_strlen_to(s3, '\0') + 2));
	if (!dst || !s1 || !s3)
	{
		if (dst)
			free(dst);
		if (s1)
			free(s1);
		if (s3)
			free(s3);
		return (NULL);
	}
	ft_strlcpy(dst, s1, ft_strlen(s1) + 1);
	if (s2)
		ft_strlcpy(&dst[ft_strlen(dst)], s2, ft_strlen(s2) + 1);
	ft_strlcpy(&dst[ft_strlen(dst)], s3, ft_strlen(s3) + 1);
	free(s1);
	free(s3);
	return (dst);
}
