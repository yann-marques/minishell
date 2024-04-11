#include "../minishell.h"

static int	check_flag(char *str, char flag);

int	ms_echo_n(t_token *token)
{
	int	k;

	k = 1;
	if (check_flag(token->value[k], 'n'))
		++k;
	while (token->value[k])
	{
		printf("%s", token->value[k]);
		if (token->value[k + 1])
			printf(" ");
		++k;
	}
	if (check_flag(token->value[1], 'n'))
		printf("\n");
	return (0);
}

static int	check_flag(char *str, char flag)
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
