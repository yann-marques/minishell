#include "minishell.h"
#include "gnl/get_next_line.h"

void perror_exit(char *str)
{
	/* char	*error_str; */

	if (str)
	{
/* 		error_str = ft_strjoin(str, "\n");
		if (!error_str)
		{
			perror("");
			exit(EXIT_FAILURE);
		} */
		perror(str);
		exit(EXIT_FAILURE);
	}
	perror("");
	exit(EXIT_FAILURE);
}


void error_exit(char *str)
{
	char	*error_str;

	if (str)
	{
		error_str = ft_strjoin(str, "\n");
		if (!error_str)
			exit(EXIT_FAILURE);
		write(2, error_str, ft_strlen(error_str));
		exit(EXIT_FAILURE);
	}
	exit(EXIT_FAILURE);
}