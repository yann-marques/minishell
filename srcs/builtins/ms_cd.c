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

static int	new_pwd(t_ms *head, t_token *token);

int	ms_cd(t_ms *head, t_token *token)
{
	char	*home;

	if (token->value[1] && token->value[2])
	{
		write(2, " too many arguments\n", 20);
		return (EXIT_FAILURE);
	}
	home = get_var_value(head, "HOME");
	if (!token->value[1] && home)
	{
		if (chdir(home) == -1)
		{
			error_str(token->value[1]);
			return (perror_str(" ", EXIT_FAILURE));
		}
		return (EXIT_SUCCESS);
	}
	if (!token->value[1])
		return (EXIT_FAILURE);
	return (new_pwd(head, token));
}

static t_env	*get_env_struct(t_env *env, char *var);

static int	new_pwd(t_ms *head, t_token *token)
{
	t_env	*pwd;
	t_env	*oldpwd;

	if (chdir(token->value[1]) == -1)
	{
		error_str(token->value[1]);
		return (perror_str(" ", EXIT_FAILURE));
	}
	pwd = get_env_struct(head->env, "PWD");
	oldpwd = get_env_struct(head->env, "OLDPWD");
	if (oldpwd && pwd)
	{
		free(oldpwd->value);
		oldpwd->value = ft_strdup(pwd->value);
	}
	if (pwd)
	{
		free(pwd->value);
		pwd->value = get_pwd(1);
	}
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
