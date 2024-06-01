/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymarques <ymarques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 17:16:58 by yanolive          #+#    #+#             */
/*   Updated: 2024/06/01 15:28:11 by ymarques         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	add_var(t_env *env, t_token *token);

int	ms_export(t_env	*env, t_token *token)
{
	t_env	*tmp;

	if (!token->value[1] && token->type != _env_var)
	{
		ms_env(env, "declare -x ");
		return (EXIT_SUCCESS);
	}
	if (token->type == _env_var)
	{
		tmp = env_new(token->value[0]);
		if (!tmp)
			return (EXIT_FAILURE);
		put_env_var(&env, tmp);
		return (EXIT_SUCCESS);
	}
	return (add_var(env, token));
}

static int	check_env_var(char *str);

static int	add_var(t_env *env, t_token *token)
{
	t_env	*tmp;
	int		i;

	i = 1;
	while (token->value[i] && check_env_var(token->value[i]))
	{
		tmp = env_new(token->value[i++]);
		if (!tmp)
			return (EXIT_FAILURE);
		put_env_var(&env, tmp);
	}
	if (token->value[i])
	{
		write(2, " not a valid identifier\n", 24);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

static int	check_env_var(char *str)
{
	int	i;

	i = 0;
	if (!ft_isalpha(str[i]) && str[i] != '_' && !quotes_jump(str))
		return (0);
	while (str[i] && str[++i] && (ft_isalpha(str[i]) || ft_isdigit(str[i])
			|| quotes_jump(&str[i]) || str[i] == '_'))
	{
		while (str[i] && quotes_jump(&str[i]))
			i += quotes_jump(&str[i]);
	}
	if (str[i] && str[i] != '=' && str[i] != ' ' && str[i] != '	')
		return (0);
	return (1);
}
