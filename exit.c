#include "minishell.h"
#include "gnl/get_next_line.h"

void perror_exit(char *str, int status)
{
	if (str)
	{
		perror(str);
		if (status != -1)
			exit(status);
		else
			exit(EXIT_FAILURE);
	}
	perror("");
	if (status != -1)
		exit(status);
	else
		exit(EXIT_FAILURE);
}


void error_exit(char *str, int status)
{
	char	*error_str;

	if (str)
	{
		error_str = ft_strjoin(str, "\n");
		if (!error_str)
			exit(EXIT_FAILURE);
		write(2, error_str, ft_strlen(error_str));
		if (status != -1)
			exit(status);
		else
			exit(EXIT_FAILURE);
	}
	if (status != -1)
		exit(status);
	else
		exit(EXIT_FAILURE);
}