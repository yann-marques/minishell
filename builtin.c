#include "minishell.h"

int	ms_echo_n(t_token *token)
{
	int	k;

	k = 1;
	while (token->value[k])
	{
		printf("%s", value[k]);
		if (value[k + 1])
			printf(" ");
		++k;
	}
	return (0);
}

int	ms_cd(t_token *token)
{
	if (chdir(token->value[1]) == -1)
	{
		printf("Error\n%s\n", strerror(errno));
		return (-1);
	}
	return (0);
}

int	ms_pwd(int count)
{
	char	*buffer;

	buffer = malloc(((BUFFER_SIZE * count) + 1) * sizeof(char));
	if (!buffer)
		return (-1);
	if (!getcwd(buffer, BUFFER_SIZE))
	{
		if (errno == ERANGE)
		{
			free(buffer);
			if (ms_pwd(count + 1) == -1)
				return (-1);
			return (0);
		}
		printf("Error\n%s", strerror(errno));
		return (-1);
	}
	printf("%s", buffer);
	free(buffer);
	return (0);
}

int	ms_export(t_env	*env, t_token *token)
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
		return (-1);
	put_env_var(env, tmp);
	return (0);
}

int	ms_unset(t_env *env, t_token *token)
{
	t_env	*tmp;
	t_env	*tmp2;

	tmp = env;
	while (tmp && tmp->next && !ft_strcmp(token->value[1], tmp->next->var))
		tmp = tmp->next;
	if (ft_strcmp(token->value[1], tmp->next->var))
	{
		tmp2 = tmp->next;
		tmp->next = tmp->next->next;
		tmp2->next = NULL;
		free_env(tmp2);
	}
	return (0);
}

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

int	ms_exit(t_ms *head)
{
	free_env(head->env);
	if (head->tokens)
		tokens_clear(head->tokens);
	free(head);
	printf("exit");
	exit(0);
}
