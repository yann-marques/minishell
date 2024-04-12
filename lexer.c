#include "minishell.h"

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

t_ms	*init_head(t_env *env)
{
	t_ms	*head;

	head = malloc(sizeof(t_ms));
	if (!head)
		return (NULL);
	head->env = env;
	head->tokens = NULL;
	head->pids = NULL;
	return (head);
}

t_ms	*lexer(t_env *env)
{
	t_ms	*head;
	char	*line;

	head = init_head(env);
	if (!head)
		return (NULL);
	line = prompt(head);
	if (!line || !check_quotes(line))
	{
		free(head);
		return (NULL);
	}
	head->tokens = set_tokens(line);
	if (!head->tokens)
	{
		free(head);
		return (NULL);
	}
	if (!var_to_value(head) || !del_quotes(head->tokens))
		ms_exit(head, NULL);
	return (head);
}