#include "minishell.h"

static int	replace_var_call(t_ms *head, char **str, int k, int i);
static int	check_if_replace(char *str, int i_var);
static char	*find_var(char *str, int i);
static char	*ms_join_three(char *s1, char *s2, char *s3);
static char	*malloc_join_three(char *s1, char *s2, char *s3);

int	var_to_value(t_ms *head)
{
	t_token	*tmp;
	int		i;
	int		k;

	tmp = head->tokens;
	while (tmp)
	{
		k = -1;
		while (tmp->value[++k])
		{
			i = -1;
			while (tmp->value[k][++i])
			{
				if (!(tmp->value[k][i] != '$' ||	(tmp->value[k][i] == '$'
						&& (tmp->value[k][i + 1] == ' '
						|| tmp->value[k][i + 1] == '\''
						|| tmp->value[k][i + 1] == '"'
						|| !tmp->value[k][i + 1])))
						&& !replace_var_call(head, tmp->value, k, i))
					return (0);
			}
		}
		tmp = tmp->next;
	}
	return (1);
}

static int	replace_var_call(t_ms *head, char **str, int k, int i)
{
	char	*var;
	char	*begin;
	char	*end;

	if (!check_if_replace(str[k], i))
		return (1);
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
	begin = ft_strndup(str[k], i);
	var = ms_join_three(begin, var, end);
	if (!var)
		return (0);
	free(str[k]);
	str[k] = var;
	return (1);
}

static int	check_if_replace(char *str, int i_var)
{
	int	i;
	int	replace;

	replace = 1;
	i = 0;
	while (str[i] && i != i_var)
	{
		if (str[i] == '\'' && ft_strchr(&str[i + 1], '\'') && ++i)
		{
			replace = 0;
			while (str[i] && str[i] != '\'' && i != i_var)
				++i;
		}
		if (!str[i] || i == i_var)
			break ;
		replace = 1;
		++i;
	}
	if (replace)
		return (1);
	return (0);
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
	int		i;
	int		j;
	int		k;

	dst = malloc_join_three(s1, s2, s3);
	if (!dst)
		return (NULL);
	i = -1;
	j = -1;
	k = -1;
	while (s1 && s1[++i])
		dst[i] = s1[i];
	while (s2 && s2[++j])
		dst[i + j] = s2[j];
	while (s3 && s3[++k])
		dst[i + j + k] = s3[k];
	dst[i + j + k] = '\0';
	free(s1);
	free(s3);
	return (dst);
}

static char	*malloc_join_three(char *s1, char *s2, char *s3)
{
	char	*dst;

	dst = malloc(sizeof(char) * (1 + ft_strlen_to(s1, '\0')
				+ ft_strlen_to(s2, '\0') + ft_strlen_to(s3, '\0')));
	if (!dst || !s1 || !s3)
	{
		if (dst)
			free(dst);
		if (s1)
			free(s1);
		if (s3)
			free(s3);
		return (0);
	}
	return (dst);
}
