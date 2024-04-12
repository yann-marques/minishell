#include "../minishell.h"

static int	check_env_var(char *str);

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
	else if (!check_env_var(token->value[1]))
	{
		write(2, " not a valid identifier", 23);
		return (EXIT_FAILURE);
	}
	else
		tmp = env_new(token->value[1]);
	if (!tmp)
		return (EXIT_FAILURE);
	put_env_var(&env, tmp);
	return (EXIT_SUCCESS);
}

static int	check_env_var(char *str)
{
	int	i;

	i = 0;
	if (!ft_isalpha(str[i++]) && !quotes_jump(str))
		return (0);
	while (str[i] && (ft_isalpha(str[i])
			|| ft_isdigit(str[i]) || quotes_jump(&str[i])))
	{
		while (str[i] && quotes_jump(&str[i]))
			i += quotes_jump(&str[i]);
		++i;
	}
	if (str[i] && str[i] != '=')
		return (0);
	return (1);
}
