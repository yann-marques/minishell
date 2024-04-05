#include "minishell.h"

void	strtab_clear(char **tab)
{
	int	k;

	k = 0;
	if (!tab)
		return ;
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

void	free_env(t_env *env)
{
	t_env	*tmp;

	tmp = env;
	while (tmp)
	{
		if (tmp->var)
			free(tmp->var);
		if (tmp->value)
			free(tmp->value);
		env = env->next;
		free(tmp);
		tmp = env;
	}
}
