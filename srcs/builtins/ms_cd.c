/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yanolive <yanolive@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 17:15:59 by yanolive          #+#    #+#             */
/*   Updated: 2024/05/28 17:23:31 by yanolive         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static t_env	*get_env_struct(t_env *env, char *var);

int	ms_cd(t_ms *head, t_token *token)
{
	t_env	*pwd;
	t_env	*oldpwd;

	if (token->value[1] && token->value[2])
		return (perror_str(" too many arguments\n", EXIT_FAILURE));
	if (!token->value[1])
		token->value[1] = ft_strdup(get_var_value(head, "HOME"));
	if (!token->value[1])
		return (EXIT_FAILURE);
	if (chdir(token->value[1]) == -1)
		return (perror_str(token->value[1], EXIT_FAILURE));
	pwd = get_env_struct(head->env, "PWD");
	oldpwd = get_env_struct(head->env, "OLDPWD");
	if (oldpwd && pwd)
		free(oldpwd->value);
	if (oldpwd && pwd)
		oldpwd->value = ft_strdup(pwd->value);
	if (pwd)
		free(pwd->value);
	if (pwd)
		pwd->value = get_pwd(1);
	free(token->value[1]);
	token->value[1] = NULL;
	return (EXIT_SUCCESS);
}

static t_env	*get_env_struct(t_env *env, char *var)
{
	t_env	*tmp;

	tmp = env;
	while (tmp)
	{
		if (!strcmp(tmp->var, var))
			return (tmp);
		tmp = tmp->next;
	}
	return (NULL);
}
