/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_cmd_type_next.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymarques <ymarques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 11:39:31 by ymarques          #+#    #+#             */
/*   Updated: 2024/05/08 16:07:04 by ymarques         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_cmd_rdout(t_token *tk)
{
	if (is_cmd(tk) && tk->next && is_rdout(tk->next))
		return (1);
	return (0);
}

int	is_cmd_rdin(t_token *tk)
{
	if (is_cmd(tk) && tk->next && is_rdin(tk->next))
		return (1);
	return (0);
}

int	is_cmd_heredoc(t_token *tk)
{
	if (is_cmd(tk) && tk->next && is_heredoc(tk->next))
		return (1);
	return (0);
}

int	next_redirect(t_token *tk)
{
	if (!tk->next)
		return (0);
	if (tk->next->type == _redirection || tk->next->type == _append)
		return (1);
	return (0);
}

int	next_redirect_out(t_token *tk)
{
	if (!tk->next)
		return (0);
	if ((tk->next->type == _redirection && tk->next->value[0][0] == '>')
		|| tk->next->type == _append)
		return (1);
	return (0);
}