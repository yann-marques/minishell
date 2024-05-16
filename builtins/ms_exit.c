/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymarques <ymarques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 17:16:06 by yanolive          #+#    #+#             */
/*   Updated: 2024/05/14 15:17:40 by ymarques         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	exit_with_token(t_ms *head, t_token *token);

int	ms_exit(t_ms *head, t_token *token)
{
	if (!token || (token->value[1] && !token->value[2]))
	{
		free_env(head->env);
		if (head->pids)
			pids_clear(head->pids);
		free(head->home);
	}
	printf("exit\n");
	if (token && token->value[1])
		return (exit_with_token(head, token));
	if (head->tokens)
		tokens_clear(head->tokens);
	close(head->original_stdint);
	free(head);
	exit(0);
}

static int	exit_with_token(t_ms *head, t_token *token)
{
	int	i;

	i = 0;
	if (token->value[1][i] == '+' || token->value[1][i] == '-')
		++i;
	while (ft_isdigit(token->value[1][i]))
		++i;
	if (token->value[1][i])
	{
		if (head->tokens)
			tokens_clear(head->tokens);
		write(2, " numeric argument required", 26);
		close(head->original_stdint);
		free(head);
		exit(2);
	}
	else if (token->value[2] && write(2, " too many arguments", 19))
		return (EXIT_FAILURE);
	i = ft_atoi(token->value[1]);
	if (head->tokens)
		tokens_clear(head->tokens);
	close(head->original_stdint);
	free(head);
	exit(i);
}
