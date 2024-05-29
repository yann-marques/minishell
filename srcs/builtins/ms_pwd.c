/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yanolive <yanolive@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 17:16:19 by yanolive          #+#    #+#             */
/*   Updated: 2024/05/28 17:03:49 by yanolive         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ms_pwd(void)
{
	char	*buffer;

	buffer = get_pwd(1);
	printf("%s\n", buffer);
	free(buffer);
	return (0);
}

char	*get_pwd(int count)
{
	char	*buffer;

	buffer = malloc(((BUFFER_SIZE * count) + 1) * sizeof(char));
	if (!buffer)
		return (NULL);
	if (!getcwd(buffer, BUFFER_SIZE * count))
	{
		free(buffer);
		if (errno == ERANGE)
			return (get_pwd(count + 1));
		perror("");
		return (NULL);
	}
	return (buffer);
}
