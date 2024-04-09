#include "../minishell.h"

int	ms_exit(t_ms *head, t_token *token)
{
	int	tmp;

	if (token && token->value[1])
		tmp = ft_atoi(token->value[1]);
	if (head)
	{
		free_env(head->env);
		if (head->tokens)
			tokens_clear(head->tokens);
		if (head->pids)
			pids_clear(head->pids);
		free(head);
	}
	if (errno == EXIT_SUCCESS)
		printf("exit\n");
	if (token && token->value[1])
		exit(tmp);
	exit(errno);
}
