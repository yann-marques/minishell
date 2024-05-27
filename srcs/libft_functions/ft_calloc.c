/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymarques <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 11:15:50 by ymarques          #+#    #+#             */
/*   Updated: 2023/11/08 16:41:26 by ymarques         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*p;
	size_t	tmp;

	if (nmemb == 0 || size == 0)
		return (ft_calloc(1, 1));
	tmp = nmemb * size;
	if (tmp / size != nmemb)
		return (NULL);
	p = (void *) malloc(nmemb * size);
	if (!p)
		return (NULL);
	ft_bzero(p, nmemb * size);
	return (p);
}
