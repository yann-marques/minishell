#include "../minishell.h"

int	ms_env(t_env *env, char *begin)
{
	t_env	*tmp;

	tmp = env;
	while (tmp)
	{
		if (!begin)
		{
			if (tmp->value[0])
				printf("%s=%s\n", tmp->var, tmp->value);
		}
		else
			printf("%s%s=%s\n", begin, tmp->var, tmp->value);
		tmp = tmp->next;
	}
	return (0);
}
