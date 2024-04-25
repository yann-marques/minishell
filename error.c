#include "minishell.h"
#include "gnl/get_next_line.h"

int perror_str(char *str, int status)
{
	if (str)
	{
		perror(str);
		return (status);
	}
	perror("");
	return (status);
}

int	error_str(char *str)
{
	if (str)
		write(2, str, ft_strlen(str));
	return (EXIT_FAILURE);
}