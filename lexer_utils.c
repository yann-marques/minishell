#include "minishell.h"

static char	*ft_strdup_noquotes(char *str);

int	check_quotes(char *str)
{
	int	i;
	int	count_splquotes;
	int	count_dblquotes;

	i = 0;
	count_splquotes = 0;
	count_dblquotes = 0;
	while (str[i])
	{
		if (str[i] == '\'')
			++count_splquotes;
		else if (str[i] == '"')
			++count_dblquotes;
		++i;
	}
	if (count_splquotes % 2 != 0 || count_dblquotes % 2 != 0)
		return (0);
	return (1);
}

int	del_quotes(t_token *tokens)
{
	t_token	*t_tmp;
	char	*s_tmp;
	int		k;

	t_tmp = tokens;
	while (t_tmp)
	{
		k = 0;
		while (t_tmp->value[k])
		{
			if (ft_strchr(t_tmp->value[k], '\'')
				|| ft_strchr(t_tmp->value[k], '"'))
			{
				s_tmp = ft_strdup_noquotes(t_tmp->value[k]);
				if (!s_tmp)
					return (0);
				free(t_tmp->value[k]);
				t_tmp->value[k] = s_tmp;
			}
			++k;
		}
		t_tmp = t_tmp->next;
	}
	return (1);
}

static char	*ft_strdup_noquotes(char *str)
{
	char	*dst;
	int		i;
	int		j;

	dst = malloc(sizeof(char) * (ft_strlen_to(str, '\0') + 1));
	if (!dst)
		return (NULL);
	i = 0;
	j = 0;
	while (str[i + j])
	{
		while (str[i + j] == '\'' || str[i + j] == '"')
			++j;
		dst[i] = str[i + j];
		++i;
	}
	dst[i] = '\0';
	return (dst);
}

int	tokens_addback(t_token **tokens, t_type type, char **value)
{
	t_token	*tmp;
	t_token	*new;

	new = malloc(sizeof(t_token));
	if (!new)
		return (-1);
	new->type = type;
	new->value = value;
	new->next = NULL;
	tmp = *tokens;
	while (tmp && tmp->next)
		tmp = tmp->next;
	if (!*tokens)
		*tokens = new;
	else
		tmp->next = new;
	return (0);
}

int	quotes_jump(char *str)
{
	int		i;
	char	c;

	if (str[0] != '\'' && str[0] != '"')
		return (0);
	c = str[0];
	i = 1;
	while (str[i] && str[i] != c)
		++i;
	return (i);
}
