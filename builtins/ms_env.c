/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yanolive <yanolive@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 17:16:14 by yanolive          #+#    #+#             */
/*   Updated: 2024/05/13 17:16:15 by yanolive         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ms_env(t_env *env, char *begin)
{
	t_env	*tmp;

	tmp = env;
	while (tmp)
	{
		if (!begin)
		{
			if (tmp->value && tmp->value[0])
				printf("%s=%s\n", tmp->var, tmp->value);
		}
		else if (ft_strcmp(tmp->var, "_") != 0)
		{
			printf("%s%s", begin, tmp->var);
			if (tmp->value && tmp->value[0])
				printf("=\"%s\"", tmp->value);
			printf("\n");
		}
		tmp = tmp->next;
	}
	return (0);
}
