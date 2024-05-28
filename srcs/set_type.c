/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_type.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymarques <ymarques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 17:10:17 by yanolive          #+#    #+#             */
/*   Updated: 2024/05/28 17:56:13 by ymarques         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_token	*redirection_type(t_token *token);
static int		check_env_var(t_token *begin_tokens, t_token *token, char *str);
static int		there_istoken(t_token *tokens, char token);

void	set_type(t_token *tokens)
{
	t_token	*tmp;

	tmp = tokens;
	while (tmp)
	{
		if (tmp->type != _none)
		{
			tmp = tmp->next;
			continue ;
		}
		else if (tmp->value[0][0] == '<' || tmp->value[0][0] == '>')
			tmp = redirection_type(tmp);
		else if (tmp->value[0][0] == '|')
			tmp->type = _pipe;
		else if (check_env_var(tokens, tmp, tmp->value[0]))
			tmp->type = _env_var;
		else
			tmp->type = _cmd_grp;
		tmp = tmp->next;
	}
}

static t_token	*redirection_type(t_token *token)
{
	if (token->value[0][0] == '<' && token->value[0][1] == '<')
		token->type = _delimiter;
	else
	{
		if (token->value[0][0] == '>' && token->value[0][1] == '>')
			token->type = _append;
		else
			token->type = _redirection;
	}
	return (token);
}

static int	check_env_var(t_token *begin_tokens, t_token *token, char *str)
{
	int	i;

	i = 0;
	if (token->value[1] || (token->next && token->next->value[0][0] != '>')
		|| there_istoken(begin_tokens, '|'))
		return (0);
	if (!ft_isalpha(str[i]) && str[i] != '_' && !quotes_jump(str))
		return (0);
	while (str[i] && str[++i] && (ft_isalpha(str[i]) || ft_isdigit(str[i])
		|| quotes_jump(&str[i]) || str[i] == '_'))
	{
		while (str[i] && quotes_jump(&str[i]))
			i += quotes_jump(&str[i]);
	}
	if (!str[i] || str[i] != '=')
		return (0);
	if (!str[++i] || str[i] == ' ' || str[i] == '	')
		return (0);
	return (1);
}

static int	there_istoken(t_token *tokens, char token)
{
	t_token	*tmp;

	tmp = tokens;
	while (tmp)
	{
		if (tmp->value[0][0] == token)
			return (1);
		tmp = tmp->next;
	}
	return (0);
}
