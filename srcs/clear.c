/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clear.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymarques <ymarques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 17:12:21 by yanolive          #+#    #+#             */
/*   Updated: 2024/05/24 14:53:54 by ymarques         ###   ########.fr       */
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

void	tokens_clear(t_token *tokens)
{
	t_token	*tmp;

	tmp = tokens;
	while (tmp)
	{
		tokens = tokens->next;
		if (tmp->value)
			strtab_clear(tmp->value);
		free(tmp);
		tmp = tokens;
	}
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

	if (head->tokens)
		tokens_clear(head->tokens);
	if (head->pids)
		pids_clear(head->pids);
	if (head->env)
		free_env(head->env);
	last_status = head->last_status;
	free(head);
	if (isExit)
		exit(last_status);
}
