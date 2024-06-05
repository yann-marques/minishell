/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymarques <ymarques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 17:12:25 by yanolive          #+#    #+#             */
/*   Updated: 2024/06/04 16:14:06 by ymarques         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	tokens_cpy(t_ms *head, t_token *tk)
{
	t_token	**tk_addr;
	t_token	*tmp;
	t_token	*tmp2;
	int		i;

	tmp = tk;
	tmp2 = tk;
	i = 0;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	tk_addr = malloc(sizeof(t_token *) * (i + 1));
	if (!tk_addr)
		exit(EXIT_FAILURE);
	i = 0;
	while (tmp2)
	{
		tk_addr[i] = tmp2;
		i++;
		tmp2 = tmp2->next;
	}
	tk_addr[i] = NULL;
	head->tokens_cpy = tk_addr;
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
		tokens_cpy(head, head->tokens);
		if (!is_handle_error(head))
			command_manager(head);
		tokens_clear(head);
		head->tokens = NULL;
	}
	free_env(head->env);
	free(head);
	return (0);
}
