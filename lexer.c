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

int	lexer(t_ms *head)
{
	char	*line;

	line = prompt(head);
	if (!line || !check_quotes(line))
		return (0);
	head->tokens = set_tokens(line, head);
	if (!head->tokens)
		return (0);
	if (!del_quotes(head->tokens))
	{
		tokens_clear(head->tokens);
		return (0);
	}
	head->token_count = 0;
	return (1);
}
