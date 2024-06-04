/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yanolive <yanolive@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 20:38:16 by ymarques          #+#    #+#             */
/*   Updated: 2024/06/04 15:00:52 by yanolive         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	calc_len(int n)
{
	int				len;
	unsigned int	temp;

	len = 0;
	temp = n;
	if (n < 0)
	{
		temp = n * -1;
		len++;
	}
	while (temp != 0)
	{
		temp = temp / 10;
		len++;
	}
	if (len == 0 && n == 0)
		return (1);
	return (len);
}

char	*ft_itoa(int n)
{
	int				len;
	long long int	temp;
	char			*str;

	temp = n;
	len = calc_len(n);
	str = (char *) ft_calloc(sizeof(char), len + 1);
	if (!str)
		return (NULL);
	len--;
	if (n < 0)
	{
		str[0] = '-';
		temp *= -1;
	}
	while (len >= 0 && str[len] != '-')
	{
		str[len] = (temp % 10) + '0';
		temp = temp / 10;
		len--;
	}
	return (str);
}
