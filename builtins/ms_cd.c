#include "../minishell.h"

int	ms_cd(t_token *token)
{
	if (token->value[2])
	{
		write(2, " too many arguments", 19);
		return (EXIT_FAILURE);
	}
	if (chdir(token->value[1]) == -1)
	{
		perror(" ");
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
