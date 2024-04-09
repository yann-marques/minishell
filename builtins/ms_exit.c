#include "../minishell.h"

int	ms_exit(t_ms *head, int status)
{
	free_env(head->env);
	if (head->tokens)
		tokens_clear(head->tokens);
	if (head->pids)
		pids_clear(head->pids);
	free(head);
	printf("exit\n");
	exit(status);
}
