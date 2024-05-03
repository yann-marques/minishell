#include "minishell.h"

t_token	*set_tokens(char *str, t_ms *head)
{
	t_token	*tokens;
	char	**tab;
	char	**tmp;
	int		k;

	tab = ms_split(str, "<|>");
	free(str);
	tab = var_to_value(tab, head);
	if (!tab)
		return (NULL);
	tokens = NULL;
	k = -1;
	while (tab[++k])
	{
		tmp = ms_split(tab[k], " ");
		if (!tmp || tokens_addback(&tokens, _none, tmp) == -1)
		{
			strtab_clear(tab);
			tokens_clear(tokens);
			return (NULL);
		}
	}
	strtab_clear(tab);
	set_type(tokens);
	return (tokens);
}

int	ft_strtab_len(char **tab)
{
	int	k;

	k = 0;
	while (tab && tab[k])
		++k;
	return (k);
}

int	join_tokens(t_token *token, t_token *extra)
{
	t_token	*tmp_token;
	char	**tab;
	int		k;
	int		q;

	k = ft_strtab_len(token->value) + ft_strtab_len(extra->next->value);
	tab = malloc(sizeof(char *) * (k + 1));
	if (!tab)
		return (0);
	k = -1;
	while (token->value[++k])
		tab[k] = token->value[k];
	free(token->value);
	q = -1;
	while (extra->next->value[++q])
		tab[k + q] = extra->next->value[q];
	free(extra->next->value);
	tab[k + q] = NULL;
	token->value = tab;
	tmp_token = extra->next;
	extra->next = extra->next->next;
	if (extra->next)
		extra->next->prev = tmp_token->prev;
	free(tmp_token);
	return (1);
}

int	reset_cmd_grp(t_token *tokens)
{
	t_token	*tmp;
	t_token	*tmp2;

	tmp = tokens;
	while (tmp)
	{
		if (tmp->type == _cmd_grp)
		{
			tmp2 = tmp;
			while (tmp2 && tmp2->next && tmp2->type != _pipe)
			{
				if (tmp2->next->type == _cmd_grp)
				{
					if (!join_tokens(tmp, tmp2))
						return (0);
				}
				tmp2 = tmp2->next;
			}
		}
		tmp = tmp->next;
	}
	return (1);
}

static char	*line_error(char *prompt_line, t_ms *head)
{
	char	*line;

	line = readline(prompt_line);
	if (!line)
	{
		free(prompt_line);
		ms_exit(head, NULL);
	}
	if (!line[0])
	{
		free(line);
		line = line_error(prompt_line, head);
		return (line);
	}
	add_history(line);
	free(prompt_line);
	return (line);
}

int	lexer(t_ms *head)
{
	char	*line;
	char	*prompt_line;

	prompt_line = prompt(head);
	line = line_error(prompt_line, head);
	if (!line || !check_quotes(line))
		return (0);
	head->tokens = set_tokens(line, head);
	if (!head->tokens)
		return (0);
	if (!del_quotes(head->tokens) || !reset_cmd_grp(head->tokens))
	{
		tokens_clear(head->tokens);
		head->tokens = NULL;
		return (0);
	}
	head->token_count = 0;
	return (1);
}
