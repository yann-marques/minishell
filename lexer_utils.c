#include "minishell.h"

int	check_quotes(char *str)
{
	int	i;
	int	error;

	i = -1;
	error = 1;
	while (str[++i] && error)
	{
		if (str[i] == '"' && ++i)
		{
			while (str[i] && str[i] != '"')
				++i;
			if (!str[i])
				error = 0;
		}
		else if (str[i] == '\'' && ++i)
		{
			while (str[i] && str[i] != '\'')
				++i;
			if (!str[i])
				error = 0;
		}
	}
	if (!error)
		free(str);
	return (error);
}

static char	*ft_strdup_noquotes(char *str);

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
	char	c;
	int		i;
	int		j;

	dst = calloc(sizeof(char), (ft_strlen_to(str, '\0') + 1));
	if (!dst)
		return (NULL);
	i = -1;
	j = 0;
	while (str[++i + j])
	{
		if ((str[i + j] == '\'' || str[i + j] == '"')
				&& ft_strchr(&str[i + j + 1], str[i + j]))
		{
			c = str[i-- + j++];
			while (str[++i + j] && str[i + j] != c)
				dst[i] = str[i + j];
			if (str[i-- + j] == c)
				++j;
			continue ;
		}
		dst[i] = str[i + j];
	}
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
	new->prev = NULL;
	tmp = *tokens;
	while (tmp && tmp->next)
		tmp = tmp->next;
	new->prev = tmp;
	if (!*tokens)
		*tokens = new;
	else
		tmp->next = new;
	return (0);
}

int	pids_addback(t_pids **pids, int pid)
{
	t_pids	*tmp;
	t_pids	*new;

	new = malloc(sizeof(t_pids));
	if (!new)
		return (-1);
	new->pid = pid;
	new->next = NULL;
	tmp = *pids;
	while (tmp && tmp->next)
		tmp = tmp->next;
	if (!*pids)
		*pids = new;
	else
		tmp->next = new;
	return (0);
}
