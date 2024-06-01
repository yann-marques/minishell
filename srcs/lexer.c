/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymarques <ymarques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 17:10:10 by yanolive          #+#    #+#             */
/*   Updated: 2024/06/01 15:24:06 by ymarques         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*set_tokens(char *str, t_ms *head)
{
	t_token	*tokens;
	char	**tab;
	char	**tmp;
	int		k;

	tab = ms_split(str, "<|>");
	free(str);
	tab = var_to_value(tab, head);
	if (!tab)
		return (NULL);
	tokens = NULL;
	k = -1;
	while (tab[++k])
	{
		tmp = ms_split(tab[k], " 	");
		if (!tmp || tokens_addback(&tokens, _none, tmp) == -1)
		{
			strtab_clear(tab);
			tokens_clear(head);
			return (NULL);
		}
	}
	strtab_clear(tab);
	set_type(tokens);
	return (tokens);
}

int	join_tokens(t_token *token, t_token *extra)
{
	t_token	*tmp_token;
	char	**tab;
	int		k;
	int		q;

	k = ft_strtab_len(token->value) + ft_strtab_len(extra->next->value);
	tab = malloc(sizeof(char *) * (k + 1));
	if (!tab)
		return (0);
	k = -1;
	while (token->value[++k])
		tab[k] = token->value[k];
	free(token->value);
	q = -1;
	while (extra->next->value[++q])
		tab[k + q] = extra->next->value[q];
	free(extra->next->value);
	tab[k + q] = NULL;
	token->value = tab;
	tmp_token = extra->next;
	extra->next = extra->next->next;
	if (extra->next)
		extra->next->prev = tmp_token->prev;
	free(tmp_token);
	return (1);
}

int	reset_cmd_grp(t_token *tokens)
{
	t_token	*tmp;
	t_token	*tmp2;

	tmp = tokens;
	while (tmp)
	{
		if (tmp->type == _cmd_grp)
		{
			tmp2 = tmp;
			while (tmp2 && tmp2->next && tmp2->type != _pipe)
			{
				if (tmp2->next->type == _cmd_grp)
				{
					if (!join_tokens(tmp, tmp2))
						return (0);
				}
				tmp2 = tmp2->next;
			}
		}
		tmp = tmp->next;
	}
	return (1);
}

int	lexer(t_ms *head)
{
	char	*line;
	char	*prompt_line;

	prompt_line = prompt(head);
	line = line_error(prompt_line, head);
	if (!line)
		return (0);
	if (!check_quotes(line))
	{
		error_str("Expected closing quotes\n");
		return (0);
	}
	head->tokens = set_tokens(line, head);
	if (!head->tokens)
		return (0);
	if (!del_quotes(head->tokens) || !reset_cmd_grp(head->tokens))
	{
		tokens_clear(head);
		head->tokens = NULL;
		return (0);
	}
	head->token_count = 0;
	return (1);
}
