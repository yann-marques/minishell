/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymarques <ymarques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 11:37:54 by ymarques          #+#    #+#             */
/*   Updated: 2024/04/01 18:41:11 by ymarques         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_strdup(const char *s)
{
	size_t	s_len;
	size_t	i;
	char	*new_string;

	if (!s)
		return (NULL);
	s_len = ft_strlen(s);
	i = 0;
	new_string = (char *) malloc((s_len + 1) * sizeof(char));
	if (!new_string || new_string == NULL)
		return (NULL);
	while (s[i] != '\0')
	{
		new_string[i] = s[i];
		i++;
	}
	new_string[i] = '\0';
	return (new_string);
}
