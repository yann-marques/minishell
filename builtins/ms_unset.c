#include "../minishell.h"

int	ms_unset(t_env *env, t_token *token)
{
	t_env	*tmp;
	t_env	*tmp2;

	tmp = env;
	if (!token->value[1])
		return (0);
	while (tmp && tmp->next && !ft_strcmp(token->value[1], tmp->next->var))
		tmp = tmp->next;
	if (ft_strcmp(token->value[1], tmp->next->var))
	{
		tmp2 = tmp->next;
		tmp->next = tmp->next->next;
		tmp2->next = NULL;
		free_env(tmp2);
	}
	return (0);
}
