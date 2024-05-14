/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_token_type.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymarques <ymarques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 11:39:31 by ymarques          #+#    #+#             */
/*   Updated: 2024/05/10 14:37:09 by ymarques         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_cmd(t_token *tk)
{
	if (tk->type == _cmd_grp)
		return (1);
	return (0);
}
int	is_rdin(t_token *tk)
{
	if (tk->value[0][1])
		return (0);
	if (tk->type == _redirection && tk->value[0][0] == '<')
		return (1);
	return (0);
}

int	is_rdout(t_token *tk)
{
	if (tk->type == _redirection && tk->value[0][0] == '>')
		return (1);
	if (tk->type == _append)
		return (1);
	return (0);
}

int	is_heredoc(t_token *tk)
{
	if (!tk)
		return (0);
	if (tk->type == _delimiter && tk->value[1])
		return (1);
	return (0);
}
