/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymarques <ymarques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 16:08:38 by ymarques          #+#    #+#             */
/*   Updated: 2024/04/05 12:08:53 by ymarques         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	while (i <= ft_strlen(s1) && i < n)
	{
		if (s1[i] == s2[i])
			i++;
		else
		{
			if (!ft_isascii(s1[i]))
				return (s2[i] - s1[i]);
			return (s1[i] - s2[i]);
		}
	}
	return (0);
}
