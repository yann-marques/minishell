/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clear.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymarques <ymarques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 17:12:21 by yanolive          #+#    #+#             */
/*   Updated: 2024/06/06 16:33:59 by ymarques         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	strtab_clear(char **tab)
{
	int	k;

	k = 0;
	if (!tab)
		return ;
	while (tab[k])
	{
		free(tab[k]);
		++k;
	}
	free(tab);
}

void	tokens_clear(t_ms *head)
{
	int		i;

	i = 0;
	if (!head->tokens_cpy)
		return ;
	while (head->tokens_cpy[i])
	{
		if (head->tokens_cpy[i]->value)
			strtab_clear(head->tokens_cpy[i]->value);
		free(head->tokens_cpy[i]);
		i++;
	}
	free(head->tokens_cpy[i]);
	free(head->tokens_cpy);
}

void	pids_clear(t_pids *pids)
{
	t_pids	*tmp;

	tmp = pids;
	while (tmp)
	{
		pids = pids->next;
		free(tmp);
		tmp = pids;
	}
}

void	free_env(t_env *env)
{
	t_env	*tmp;

	tmp = env;
	while (tmp)
	{
		if (tmp->var)
			free(tmp->var);
		if (tmp->value)
			free(tmp->value);
		env = env->next;
		free(tmp);
		tmp = env;
	}
}

void	exit_free_head(t_ms *head, int isExit)
{
	int		last_status;

	close(head->original_stdint);
	close(head->heredoc_stdint);
	if (head->tokens)
		tokens_clear(head);
	if (head->pids)
		pids_clear(head->pids);
	if (head->env)
		free_env(head->env);
	last_status = head->last_status;
	free(head);
	if (isExit)
		exit(last_status);
}
