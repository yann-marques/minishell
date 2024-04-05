#include "../minishell.h"

int	ms_echo_n(t_token *token)
{
	int	k;
	int	i;

	k = 1;
	i = 0;
	while (token->value[k])
	{
		while (token->value[k][i])
		{
			while (token->value[k][i] == '\''
				|| token->value[k][i] == '"')
				++i;
			if (token->value[k][i])
				write(1, &token->value[k][i++], 1);
		}
		if (token->value[k + 1])
			printf(" ");
		++k;
	}
	return (0);
}
