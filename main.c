/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymarques <ymarques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 17:12:25 by yanolive          #+#    #+#             */
/*   Updated: 2024/05/24 14:55:04 by ymarques         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	display_tokens(t_token *tokens);

t_ms	*init_head(char **envp)
{
	t_ms	*head;

	head = malloc(sizeof(t_ms));
	if (!head)
		return (NULL);
	head->env = set_env(envp);
	head->tokens = NULL;
	head->pids = NULL;
	head->last_status = 0;
	head->original_stdint = 0;
	return (head);
}

int	main(int ac, char **av, char **envp)
{
	t_ms	*head;

	if (ac != 1)
		return (-1);
	(void)av;
	head = init_head(envp);
	if (!head)
		return (-1);
	sig_control(1);
	while (1)
	{
		if (!head->tokens && !lexer(head))
			continue ;
		if (!is_handle_error(head))
			command_manager(head);
		tokens_clear(head->tokens);
		head->tokens = NULL;
	}
	free_env(head->env);
	free(head);
	return (0);
}
