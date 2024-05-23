/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yanolive <yanolive@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 17:10:03 by yanolive          #+#    #+#             */
/*   Updated: 2024/05/22 16:31:31 by yanolive         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void		add_or_replace(t_env *env, t_env *new);

t_env	*set_env(char **env)
{
	t_env	*s_env;
	t_env	*tmp;
	int		k;

	s_env = NULL;
	k = 0;
	while (env && env[k])
	{
		tmp = env_new(env[k]);
		if (!tmp)
		{
			free_env(s_env);
			return (NULL);
		}
		put_env_var(&s_env, tmp);
		++k;
	}
	return (s_env);
}

char	*get_var_value(t_ms *head, char *var)
{
	t_env	*tmp;

	tmp = head->env;
	if (ft_strncmp(var, "?", 2) == 0)
		return (ft_itoa(head->last_status));
	while (tmp)
	{
		if (ft_strcmp(tmp->var, var) == 0)
			return (tmp->value);
		tmp = tmp->next;
	}
	return (NULL);
}

t_env	*env_new(char *env)
{
	t_env	*new;

	new = malloc(sizeof(t_env));
	if (!new)
		return (NULL);
	new->var = ft_strndup(env, ft_strlen_to(env, '='));
	if (!new->var)
	{
		free(new);
		return (NULL);
	}
	new->next = NULL;
	new->value = NULL;
	if (!env[ft_strlen_to(env, '=')])
		return (new);
	new->value = ft_strndup(&env[ft_strlen_to(env, '=') + 1], 0);
	if (!new->value)
	{
		free(new->var);
		free(new);
		return (NULL);
	}
	return (new);
}

void	put_env_var(t_env **env, t_env *new)
{
	t_env	*tmp;
	int		i;

	i = 0;
	tmp = *env;
	if (!*env)
	{
		*env = new;
		return ;
	}
	if (ft_strcmp(tmp->var, new->var) > 0)
	{
		new->next = *env;
		*env = new;
		return ;
	}
	while (tmp && tmp->next)
	{
		i = ft_strcmp(tmp->var, new->var);
		if (i == 0 || (i < 0 && ft_strcmp(tmp->next->var, new->var) > 0))
			break ;
		else
			tmp = tmp->next;
	}
	add_or_replace(tmp, new);
}

static void	add_or_replace(t_env *env, t_env *new)
{
	t_env	*tmp;

	if (((env->next && ft_strcmp(env->next->var, new->var) == 0)
			|| ft_strcmp(env->var, new->var) == 0) && !new->value)
		return ;
	if (env->next && ft_strcmp(env->next->var, new->var) == 0)
	{
		tmp = env->next;
		new->next = tmp->next;
		env->next = new;
		free(tmp->var);
		free(tmp->value);
		free(tmp);
		return ;
	}
	if (ft_strcmp(env->var, new->var) == 0)
	{
		free(env->value);
		env->value = new->value;
		free(new->var);
		free(new);
		return ;
	}
	new->next = env->next;
	env->next = new;
}
