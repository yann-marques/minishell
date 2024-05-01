#include "../minishell.h"

int	ms_unset(t_env *env, t_token *token)
{
	t_env	*tmp;
	t_env	*tmp2;
	int		i;

	i = 1;
	while (token->value[i])
	{
		tmp = env;
		while (tmp && tmp->next && ft_strcmp(token->value[i], tmp->next->var))
			tmp = tmp->next;
		if (ft_strcmp(token->value[i], tmp->next->var) == 0)
		{
			tmp2 = tmp->next;
			tmp->next = tmp->next->next;
			tmp2->next = NULL;
			free_env(tmp2);
		}
		++i;
	}
	return (0);
}
