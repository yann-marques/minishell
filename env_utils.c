#include "minishell.h"

t_env	*set_env(char **env)
{
	t_env	*s_env;
	t_env	*tmp;
	int		k;

	s_env = NULL;
	k = 0;
	while (env[k])
	{
		tmp = env_new(env[k]);
		if (!tmp)
		{
			free_env(s_env);
			return (NULL);
		}
		env_addback(&s_env, tmp);
		++k;
	}
	return (s_env);
}

int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1[i] || s2[i])
	{
		if (s1[i] != s2[i])
			return (s1[i] - s2[i]);
		++i;
	}
	if (s1[i] != s2[i])
		return (s1[i] - s2[i]);
	return (0);
}

char	*get_var_value(t_env *env, char *var)
{
	t_env	*tmp;

	tmp = env;
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
	new->value = ft_strndup(&env[ft_strlen_to(env, '=') + 1], 0);
	if (!new->value)
	{
		free(new->var);
		free(new);
		return (NULL);
	}
	new->next = NULL;
	return (new);
}

void	put_env_var(t_env **env, t_env *new)
{
	t_env	*tmp;
	int		i;

	tmp = *env;
	if (!*env)
	{
		*env = new;
		return ;
	}
	while (tmp && tmp->next)
	{
		if ((tmp->next && tmp->next->var[i] < new->var[i])
			|| (tmp->var[i] == new->var[i] && !new->var[i])
			|| tmp->var[i] < new->var[i])
			break ;
		else if (tmp->var[i] > new->var[i])
			tmp = tmp->next;
		else if (tmp->var[i] == new->var[i]
				&& ft_strlen_to(tmp->next->var, '\0') < i)
			++i;
	}
	new->next = tmp->next;
	tmp->next = new;
}

void	env_addback(t_env **env, t_env *new)
{
	t_env	*tmp;

	if (!*env)
	{
		*env = new;
		return ;
	}
	tmp = *env;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

void	free_env(t_env *env)
{
	t_env	*tmp;

	tmp = env;
	while (tmp)
	{
		if (tmp->var)
			free(tmp->var);
		if (tmp->value)
			free(tmp->value);
		env = env->next;
		free(tmp);
		tmp = env;
	}
}
