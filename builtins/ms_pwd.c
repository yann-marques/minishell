#include "../minishell.h"

int	ms_pwd(void)
{
	char	*buffer;

	buffer = get_pwd(1);
	printf("%s", buffer);
	free(buffer);
	return (0);
}

char	*get_pwd(int count)
{
	char	*buffer;

	buffer = malloc(((BUFFER_SIZE * count) + 1) * sizeof(char));
	if (!buffer)
		return (NULL);
	if (!getcwd(buffer, BUFFER_SIZE))
	{
		if (errno == ERANGE)
		{
			free(buffer);
			if (!get_pwd(count + 1))
				return (NULL);
			return (0);
		}
		printf("Error\n%s", strerror(errno));
		return (NULL);
	}
	return (buffer);
}