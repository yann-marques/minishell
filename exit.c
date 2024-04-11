#include "minishell.h"
#include "gnl/get_next_line.h"

void error_exit(char *str)
{
	printf("%s\n", str);
	exit(EXIT_FAILURE);
}