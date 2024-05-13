/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yanolive <yanolive@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 17:15:59 by yanolive          #+#    #+#             */
/*   Updated: 2024/05/13 17:16:00 by yanolive         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ms_cd(t_token *token)
{
	if (token->value[2])
	{
		write(2, " too many arguments", 19);
		return (EXIT_FAILURE);
	}
	if (chdir(token->value[1]) == -1)
	{
		perror(" ");
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
