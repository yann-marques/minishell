/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move_rdout.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymarques <ymarques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 11:39:31 by ymarques          #+#    #+#             */
/*   Updated: 2024/05/16 13:16:51 by ymarques         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	if_rdout(t_token **tk, t_token **out, t_token **head)
{
	if (is_rdout(*tk))
	{
		*out = *tk;
		if ((*tk)->next && !(*tk)->prev)
			*head = (*tk)->next;
		if ((*tk)->next && (*tk)->next->type != _pipe && (*tk)->prev
			&& !is_rdout((*tk)->next) && !is_rdout((*tk)->prev))
			(*tk)->prev->next = (*tk)->next;
	}
}

void	move_rdout(t_token **tk)
{
	t_token	*head;
	t_token	*out;

	head = *tk;
	out = NULL;
	while (*tk)
	{
		if_rdout(tk, &out, &head);
		if ((!(*tk)->next || (*tk)->next->type == _pipe) && out && out != *tk)
		{
			out->prev = *tk;
			if ((*tk)->next)
				out->next = (*tk)->next;
			else
				out->next = NULL;
			(*tk)->next = out;
			*tk = out;
			out = NULL;
		}
		if ((*tk)->type == _pipe)
			out = NULL;
		*tk = (*tk)->next;
	}
	*tk = head;
}
