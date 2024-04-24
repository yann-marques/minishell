#include "minishell.h"
#include "gnl/get_next_line.h"

int perror_str(char *str, int status)
{
	if (str)
	{
		perror(str);
		if (status != -1)
			return (status);
		else
			return (EXIT_FAILURE);
	}
	perror("");
	if (status != -1)
		return (status);
	else
		return (EXIT_FAILURE);
}

int	error_str(char *str)
{
	if (str)
		write(2, str, ft_strlen(str));
	return (EXIT_FAILURE);
}