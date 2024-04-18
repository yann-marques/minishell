#include "minishell.h"
#include "gnl/get_next_line.h"

int perror_str(char *str)
{
	//char	*error_str;

	if (str)
	{
/* 		error_str = ft_strjoin(str, "\n");
		if (!error_str)
		{
			perror("");
			return (EXIT_FAILURE);
		} */
		perror(str);
		return (EXIT_FAILURE);
	}
	perror("");
	return (EXIT_FAILURE);
}

int	error_str(char *str)
{
	if (str)
		write(2, str, ft_strlen(str));
	return (EXIT_FAILURE);
}