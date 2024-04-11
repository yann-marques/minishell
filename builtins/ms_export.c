#include "../minishell.h"

int	ms_export(t_env	*env, t_token *token)
{
	t_env	*tmp;

	if (!token->value[1] && token->type != _env_var)
	{
		ms_env(env, "declare -x ");
		return (EXIT_SUCCESS);
	}
	if (token->type == _env_var)
		tmp = env_new(token->value[0]);
	else
		tmp = env_new(token->value[1]);
	if (!tmp)
		return (EXIT_FAILURE);
	put_env_var(&env, tmp);
	return (EXIT_SUCCESS);
}
