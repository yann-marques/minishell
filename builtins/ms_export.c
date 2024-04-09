#include "../minishell.h"

int	ms_export(t_ms *head, t_env	*env, t_token *token)
{
	t_env	*tmp;

	if (!token->value[1] && token->type != _env_var)
	{
		ms_env(env, "declare -x ");
		return (0);
	}
	if (token->type == _env_var)
		tmp = env_new(token->value[0]);
	else
		tmp = env_new(token->value[1]);
	if (!tmp)
		error_exit(head, NULL);
	put_env_var(&env, tmp);
	return (0);
}
