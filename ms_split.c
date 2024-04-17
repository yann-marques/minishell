#include "minishell.h"

static int	ms_split_size(char *str, char *sep);
static int	get_next_sep(char *str, char *sep);

char	**ms_split(char *str, char *sep)
{
	char	**tab;
	int		i;
	int		j;
	int		k;

	tab = malloc(sizeof(char *) * ms_split_size(str, sep));
	if (!tab)
		return (NULL);
	i = 0;
	k = -1;
	while (str[i] && (i == 0 || str[i - 1]))
	{
		if (str[i] == ' ' && ++i)
			continue ;
		j = get_next_sep(&str[i], sep);
		tab[++k] = ft_strndup(&str[i], j);
		if (!tab[k])
		{
			strtab_clear(tab);
			return (NULL);
		}
		i += j;
	}
	tab[++k] = NULL;
	return (tab);
}

static int	ms_split_size(char *str, char *sep)
{
	int	i;
	int	count;

	i = 0;
	count = 1;
	while (str[i])
	{
		i += quotes_jump(&str[i]);
		if (ft_strchr(sep, str[i]) && str[i] == str[i + 1] && str[i] == '|')
		{
			++i;
			count += 2;
		}
		else if (ft_strchr(sep, str[i]))
			++count;
		else if (i == 0 || ft_strchr(sep, str[i - 1]))
			++count;
		if (str[i])
			++i;
	}
	return (count + 1);
}

static int	get_next_sep(char *str, char *sep)
{
	int	i;

	i = 0;
	if (ft_strchr(sep, str[i]))
	{
		++i;
		if (ft_strchr(sep, str[i]))
			++i;
		if (str[0] == '<' || str[0] == '>')
		{
			while (str[i] == ' ')
				++i;
			//i += get_next_sep(&str[i], " ");
			i += get_next_sep(&str[i], sep); //fix pour patch un probleme de parsing:  cat <./test_files/infile_big ./test_files/infile
		}
		return (i);
	}
	if (sep[0] == ' ' && (str[0] == '<' || str[0] == '>'))
	{
		while (str[i] == str[0])
			++i;
		return (i);
	}
	while (str[i] && !ft_strchr(sep, str[i]))
		i += quotes_jump(&str[i]) + 1;
	return (i);
}
