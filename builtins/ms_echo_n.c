#include "../minishell.h"

int	ms_echo_n(t_token *token)
{
	int	k;

	k = 2;
	while (token->value[k])
	{
		printf("%s", token->value[k]);
		if (token->value[k + 1])
			printf(" ");
		++k;
	}
	return (0);
}
