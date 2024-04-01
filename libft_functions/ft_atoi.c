/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymarques <ymarques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 17:49:34 by ymarques          #+#    #+#             */
/*   Updated: 2024/04/01 18:39:23 by ymarques         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	ft_isspace(char c)
{
	return ((c >= 8 && c <= 13) || c == ' ');
}

int	ft_atoi(const char *nptr)
{
	size_t	i;
	int		sign;
	int		value;

	i = 0;
	sign = 1;
	value = 0;
	while (ft_isspace(nptr[i]))
		i++;
	if (nptr[i] == '-' || nptr[i] == '+')
	{
		if (nptr[i] == '-')
			sign = -1;
		i++;
	}
	while (ft_isdigit(nptr[i]))
	{
		value = (value * 10) + nptr[i] - '0';
		i++;
	}
	return (value * sign);
}
