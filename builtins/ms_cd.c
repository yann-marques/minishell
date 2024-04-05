#include "../minishell.h"

int	ms_cd(t_token *token)
{
	if (chdir(token->value[1]) == -1)
	{
		printf("Error\n%s\n", strerror(errno));
		return (-1);
	}
	return (0);
}
