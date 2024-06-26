/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymarques <ymarques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 11:39:31 by ymarques          #+#    #+#             */
/*   Updated: 2024/05/28 14:41:56 by ymarques         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../gnl/get_next_line.h"

int	env_size(t_env *env)
{
	t_env	*tmp;
	int		count;

	tmp = env;
	count = 0;
	while (tmp)
	{
		count++;
		tmp = tmp->next;
	}
	return (count);
}

int	heredoc_size(t_heredoc *here_doc)
{
	t_heredoc	*tmp;
	int			count;

	tmp = here_doc;
	count = 0;
	while (tmp)
	{
		count++;
		tmp = tmp->next;
	}
	return (count);
}

char	**t_env_to_strtab(t_env *env)
{
	char	**envp;
	t_env	*tmp_env;
	char	*tmp;
	int		k;

	envp = malloc(sizeof(char *) * (env_size(env) + 1));
	if (!envp)
		return (NULL);
	tmp_env = env;
	k = 0;
	while (tmp_env)
	{
		if (tmp_env->value)
		{
			tmp = ft_strjoin(tmp_env->var, "=");
			envp[k] = ft_strjoin(tmp, tmp_env->value);
			if (tmp)
				free(tmp);
			if (!envp[k++])
				return (strtab_clear(envp), NULL);
		}
		tmp_env = tmp_env->next;
	}
	envp[k] = NULL;
	return (envp);
}

char	**t_heredoc_to_strtab(t_heredoc *heredoc)
{
	t_heredoc	*prev;
	char		**tab;
	int			k;

	tab = malloc(sizeof(char *) * (heredoc_size(heredoc) + 1));
	if (!tab)
		return (NULL);
	k = 0;
	while (heredoc)
	{
		tab[k] = ft_strdup(heredoc->line);
		free(heredoc->line);
		++k;
		prev = heredoc;
		heredoc = heredoc->next;
		free(prev);
	}
	tab[k] = NULL;
	return (tab);
}

t_token	*get_n_token(t_token *tokens, int count)
{
	t_token	*tmp;
	int		i;

	tmp = tokens;
	i = 0;
	while (tmp && i < count)
	{
		tmp = tmp->next;
		++i;
	}
	return (tmp);
}
