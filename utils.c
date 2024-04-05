#include "minishell.h"

char	*ft_strndup(char *str, int n)
{
	char	*dst;
	int		i;

	if (n == -1)
		n = 0;
	else if (n == 0)
		n = ft_strlen_to(str, '\0');
	dst = malloc(sizeof(char) * (n + 1));
	if (!dst)
		return (NULL);
	i = 0;
	while (str[i] && i < n)
	{
		dst[i] = str[i];
		++i;
	}
	dst[i] = '\0';
	return (dst);
}

char	*ft_strchr(char *str, char c)
{
	int	i;

	i = 0;
	while (str && str[i])
	{
		if (str[i] == c)
			return (&str[i]);
		++i;
	}
	return (NULL);
}

int	ft_strlen_to(char *str, char c)
{
	int	i;

	i = 0;
	while (str && str[i] && str[i] != c)
		++i;
	return (i);
}

int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1[i] || s2[i])
	{
		if (s1[i] != s2[i])
			return (s1[i] - s2[i]);
		++i;
	}
	if (s1[i] != s2[i])
		return (s1[i] - s2[i]);
	return (0);
}
