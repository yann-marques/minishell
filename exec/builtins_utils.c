#include "../minishell.h"
#include "../gnl/get_next_line.h"

int	check_flag(char *str, char flag)
{
	int	i;

	i = 0;
	if (str[i++] != '-')
		return (0);
	if (str[i++] != flag)
		return (0);
	while (str[i])
	{
		if (str[i] != flag)
			return (0);
		++i;
	}
	return (1);
}

int	check_echo_builtin(char **value)
{
	if (ft_strcmp(value[0], "echo") != 0)
		return (0);
	if (!value[1])
		return (1);
	if (check_flag(value[1] ,'e') || check_flag(value[1] ,'E'))
		return (0);
	if (check_flag(value[1] ,'n') && value[2]
			&& (check_flag(value[2] ,'e') || check_flag(value[2] ,'E')))
		return (0);
	return (1);
}

int	check_if_builtins_parent(t_ms *head, t_token *token)
{
	//EXIT ET CD DOIVENT etre execute dans le parent (mais il faut les laisser dans builtin_child()). peut-etre aussi export et unset mais j'ai pas pu test ils ont lair de pas fonctionner.
	if (ft_strcmp(token->value[0], "exit") == 0)
	{
		head->last_status = ms_exit(head, token);
		return (1);
	}
	if (ft_strcmp(token->value[0], "cd") == 0)
	{
		head->last_status = ms_cd(token);
		return (1);
	}
	if (ft_strcmp(token->value[0], "export") == 0)
	{
		head->last_status = ms_export(head->env, token);
		return (1);
	}
	return (0);
}

int	builtin_child(t_ms *head, t_token *token)
{
	int	status;

	if (check_echo_builtin(token->value))
		status = ms_echo_n(token);
	else if (ft_strcmp(token->value[0], "cd") == 0)
		status = ms_cd(token);
	else if  (ft_strcmp(token->value[0], "pwd") == 0)
		status = ms_pwd();
	else if  (ft_strcmp(token->value[0], "export") == 0)
		status = ms_export(head->env, token);
	else if  (ft_strcmp(token->value[0], "unset") == 0)
		status = ms_unset(head->env, token);
	else if  (ft_strcmp(token->value[0], "env") == 0 && !token->value[1])
		status = ms_env(head->env, NULL);
	else if (ft_strcmp(token->value[0], "exit") == 0)
		status = ms_exit(head, token);
	else
		return (0);
	head->last_status = status;
	return (1);
}
