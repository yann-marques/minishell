#include "minishell.h"

int	ft_strlen_to(char *str, char c)
{
	int	i;

	i = 0;
	while (str && str[i] && str[i] != c)
		++i;
	return (i);
}

void	strtab_clear(char **tab)
{
	int	k;

	k = 0;
	while (tab[k])
	{
		free(tab[k]);
		++k;
	}
	free(tab);
}

void	tokens_clear(t_token *tokens)
{
	t_token	*tmp;

	tmp = tokens;
	while (tmp)
	{
		tokens = tokens->next;
		if (tmp->value)
			strtab_clear(tmp->value);
		free(tmp);
		tmp = tokens;
	}
}

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

char	*ft_strchr(char *str, char c)
{
	int	i;

	i = 0;
	while (str && str[i])
	{
		if (str[i] == c)
			return (&str[i]);
		++i;
	}
	return (NULL);
}

int	ms_split_size(char *str, char *sep)
{
	int	i;
	int	count;

	i = 0;
	count = 1;
	while (str[i])
	{
		i += quotes_jump(&str[i]);
		if (ft_strchr(sep, str[i]) && str[i] == str[i + 1] && str[i] == '|')
		{
			++i;
			count += 2;
		}
		else if (ft_strchr(sep, str[i]))
			++count;
		else if (i == 0 || ft_strchr(sep, str[i - 1]))
			++count;
		if (str[i])
			++i;
	}
	return (count + 1);
}

int	get_next_sep(char *str, char *sep)
{
	int	i;

	i = 0;
	if (ft_strchr(sep, str[i]))
	{
		++i;
		if (ft_strchr(sep, str[i]))
			++i;
		if (str[0] == '<' && str[1] == '<')
		{
			while (str[i] == ' ')
				++i;
			i += get_next_sep(&str[i], " ");
		}
		return (i);
	}
	while (str[i] && !ft_strchr(sep, str[i]))
	{
		i += quotes_jump(&str[i]);
		++i;
	}
	return (i);
}

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

char	**ms_split(char *str, char *sep)
{
	char	**tab;
	int		i;
	int		j;
	int		k;

	tab = malloc(sizeof(char *) * ms_split_size(str, sep));
	if (!tab)
		return (NULL);
	i = 0;
	k = -1;
	while (str[i] && (i == 0 || str[i - 1]))
	{
		if (str[i] == ' ' && ++i)
			continue ;
		j = get_next_sep(&str[i], sep);
		tab[++k] = ft_strndup(&str[i], j);
		if (!tab[k])
		{
			strtab_clear(tab);
			return (NULL);
		}
		i += j;
	}
	tab[++k] = NULL;
	return (tab);
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

t_token	*set_tokens(char *str)
{
	t_token	*tokens;
	char	**tab;
	char	**tmp;
	int		k;

	tab = ms_split(str, "<|>");
	free(str);
	if (!tab)
		return (NULL);
	tokens = NULL;
	k = 0;
	while (tab[k])
	{
		tmp = ms_split(tab[k], " ");
		if (!tmp || tokens_addback(&tokens, _none, tmp) == -1)
		{
			strtab_clear(tab);
			tokens_clear(tokens);
			return (NULL);
		}
		++k;
	}
	strtab_clear(tab);
	set_type(tokens);
	return (tokens);
}

int	ft_isalpha(char c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		return (1);
	return (0);
}

int	ft_isdigit(char c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

int	ft_aretoken(t_token *tokens, char token)
{
	t_token	*tmp;

	tmp = tokens;
	while (tmp)
	{
		if (tmp->value[0][0] == token)
			return (1);
		tmp = tmp->next;
	}
	return (0);
}

int	check_env_var(t_token *begin_tokens, t_token *token, char *str)
{
	int	i;

	i = 0;
	if (token->value[1] || (token->next && token->next->value[0][0] != '>')
		|| ft_aretoken(begin_tokens, '|'))
		return (0);
	if (!ft_isalpha(str[i++]) && !quotes_jump(str))
		return (0);
	while (str[i] && (ft_isalpha(str[i])
			|| ft_isdigit(str[i]) || quotes_jump(&str[i])))
	{
		while (str[i] && quotes_jump(&str[i]))
			i += quotes_jump(&str[i]);
		++i;
	}
	if (!str[i] || str[i] != '=')
		return (0);
	if (!str[++i] || str[i] == ' ')
		return (0);
	return (1);
}

t_token	*redirection_type(t_token *token)
{
	if (token->value[0][0] == '<' && token->value[0][1] == '<')
		token->type = _delimiter;
	else
	{
		if (token->value[0][0] == '>' && token->value[0][1] == '>')
			token->type = _append;
		else
			token->type = _redirection;
		if (token->next)
			token->next->type = _file;
	}
	return (token);
}

void	set_type(t_token *tokens)
{
	t_token	*tmp;

	tmp = tokens;
	while (tmp)
	{
		if (tmp->type != _none)
		{
			tmp = tmp->next;
			continue ;
		}
		else if (tmp->value[0][0] == '<' || tmp->value[0][0] == '>')
			tmp = redirection_type(tmp);
		else if (tmp->value[0][0] == '|')
			tmp->type = _pipe;
		else if (check_env_var(tokens, tmp, tmp->value[0]))
			tmp->type = _env_var;
		else
			tmp->type = _cmd_grp;
		tmp = tmp->next;
	}
}

char	*find_var(char *str, int i)
{
	char	*var;
	int		tmp;

	tmp = ft_strlen_to(&str[i + 1], ' ');
	if (tmp > ft_strlen_to(&str[i + 1], '"'))
		tmp = ft_strlen_to(&str[i + 1], '"');
	var = ft_strndup(&str[i + 1], tmp);
	if (!var)
		return (NULL);
	return (var);
}

char	*malloc_join_three(char *s1, char *s2, char *s3)
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

char	*ms_join_three(char *s1, char *s2, char *s3)
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

int	replace_var_call(t_env *env, char **str, int k, int i)
{
	char	*var;
	char	*begin;
	char	*end;

	if (str[k][i] != '$')
		return (1);
	begin = find_var(str[k], i);
	if (!begin)
		return (0);
	var = get_var_value(env, begin);
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
				if (tmp->value[k][i] == '\'' && ++i)
				{
					while (tmp->value[k][i] != '\'')
						++i;
				}
				if (!replace_var_call(head->env, tmp->value, k, i))
					return (0);
			}
		}
		tmp = tmp->next;
	}
	return (1);
}

char	*get_username(t_env *env)
{
    char	*username;

    username = get_var_value(env, "USER");
    if (!username)
        return ("anonynous");
    return (username);   
}

char	*prompt(t_env *env)
{
	char	*pwd;
	char	*absolute;
    char    *username;
	char	*line;
	int		i;

	pwd = get_pwd(1);
	if (!pwd)
		return (NULL);
	absolute = get_var_value(env, "HOME");
	if (!absolute)
	{
		free(pwd);
		return (NULL);
	}
	i = 0;
	while (absolute[i])
		++i;
    username = get_username(env);
	printf("\033[1;32m%s\033[1;0m\033[1;0m@\033[1;36mminishell\033[1;0m:\033[1;33m~%s\033[1;0m", username, &pwd[i]);
	free(pwd);
	line = readline("$ ");
	if (!line)
		return (NULL);
	return (line);
}

char	*ft_strdup_noquotes(char *str)
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

t_ms	*lexer(t_env *env)
{
	t_ms	*head;
	char	*line;

	line = prompt(env);
	if (!line)
		return (NULL);
	add_history(line);
	head = malloc(sizeof(t_ms));
	if (!head || !check_quotes(line))
	{
		free(line);
		if (head)
			free(head);
		return (NULL);
	}
	head->tokens = set_tokens(line);
	if (!head->tokens)
	{
		free(head);
		return (NULL);
	}
	head->env = env;
	if (!var_to_value(head) || !del_quotes(head->tokens))
		ms_exit(head);
	return (head);
}
