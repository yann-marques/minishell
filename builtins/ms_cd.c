/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymarques <ymarques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 17:15:59 by yanolive          #+#    #+#             */
/*   Updated: 2024/05/16 11:51:23 by ymarques         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ms_cd(t_token *token)
{
	if (token->value[2])
	{
		write(2, " too many arguments\n", 20);
		return (EXIT_FAILURE);
	}
	if (chdir(token->value[1]) == -1)
	{
		error_str(token->value[1]);
		return (perror_str(" ", EXIT_FAILURE));
	}
	return (EXIT_SUCCESS);
}
