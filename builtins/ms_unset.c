/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yanolive <yanolive@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 17:16:54 by yanolive          #+#    #+#             */
/*   Updated: 2024/05/13 17:16:55 by yanolive         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ms_unset(t_env *env, t_token *token)
{
	t_env	*tmp;
	t_env	*tmp2;
	int		i;

	i = 1;
	while (token->value[i])
	{
		tmp = env;
		while (tmp && tmp->next && ft_strcmp(token->value[i], tmp->next->var))
			tmp = tmp->next;
		if (tmp->next && ft_strcmp(token->value[i], tmp->next->var) == 0)
		{
			tmp2 = tmp->next;
			tmp->next = tmp->next->next;
			tmp2->next = NULL;
			free_env(tmp2);
		}
		++i;
	}
	return (0);
}
