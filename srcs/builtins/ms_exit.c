/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymarques <ymarques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 17:16:06 by yanolive          #+#    #+#             */
/*   Updated: 2024/05/28 11:54:35 by ymarques         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	exit_with_token(t_ms *head, t_token *token);

int	ms_exit(t_ms *head, t_token *token)
{
	int	last_status;
	printf("exit\n");
	if (token && token->value[1])
		return (exit_with_token(head, token));
	if (head->tokens)
		tokens_clear(head->tokens);
	free_env(head->env);
	if (head->pids)
		pids_clear(head->pids);
	close(head->original_stdint);
	last_status = head->last_status;
	free(head);
	exit(last_status);
}

static int	ft_isspace(char c)
{
	return ((c >= 8 && c <= 13) || c == ' ');
}

static __int128	safe_atoi(const char *nptr, bool *error)
{
	size_t		i;
	int			sign;
	__int128	value;

	i = 0;
	sign = 1;
	value = 0;
	*error = false;
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
	if (value > LLONG_MAX || value < LLONG_MIN)
		*error = true;
	return (value * sign);
}

static int	exit_with_token(t_ms *head, t_token *token)
{
	int			i;
	bool		error;

	i = 0;
	error = false;
	if (token->value[1][i] == '+' || token->value[1][i] == '-')
		++i;
	while (ft_isdigit(token->value[1][i]))
		++i;
	if (token->value[1][i])
	{
		write(2, " numeric argument required\n", 27);
		close(head->original_stdint);
		exit_free_head(head, 0);
		exit(2);
	}
	else if (token->value[2] && write(2, " too many arguments\n", 20))
		return (EXIT_FAILURE);
	i = safe_atoi(token->value[1], &error);
	if (error)
		error_exit(head, "numeric argument required", 2);
	close(head->original_stdint);
	exit_free_head(head, 0);
	exit(i);
}
