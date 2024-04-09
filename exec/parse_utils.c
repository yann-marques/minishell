#include "../minishell.h"
#include "../gnl/get_next_line.h"

int	env_size(t_env *env)
{
	t_env	*tmp;
	int		count;

	tmp = env;
	count = 0;
	while (tmp)
	{
		tmp = tmp->next;
		++count;
	}
	return (count);
}

char	**t_env_to_strtab(t_env *env)
{
	char	**envp;
	t_env	*tmp_env;
	char	*tmp;
	int		k;

	envp = malloc(sizeof(char *) * (env_size(env) + 1));
	if (!envp)
		return (NULL);
	tmp_env = env;
	k = 0;
	while (tmp_env)
	{
		tmp = ft_strjoin(tmp_env->var, "=");
		envp[k] = ft_strjoin(tmp, tmp_env->value);
		if (tmp)
			free(tmp);
		if (!envp[k])
		{
			strtab_clear(envp);
			return (NULL);
		}
		++k;
		tmp_env = tmp_env->next;
	}
	envp[k] = NULL;
	return (envp);
}

t_token	*get_n_token(t_token *tokens, int count)
{
	t_token	*tmp;
	int		i;

	tmp = tokens;
	i = 0;
	while (tmp && i < count)
	{
		tmp = tmp->next;
		++i;
	}
	return (tmp);
}