/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yanolive <yanolive@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 17:12:25 by yanolive          #+#    #+#             */
/*   Updated: 2024/05/13 17:12:26 by yanolive         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	display_tokens(t_token *tokens);

t_ms	*send_head(t_ms *new_head)
{
	static t_ms	*head = NULL;
	t_ms		*tmp;

	tmp = head;
	head = new_head;
	return (tmp);
}

t_ms	*init_head(char **envp)
{
	t_ms	*head;

	head = malloc(sizeof(t_ms));
	if (!head)
		return (NULL);
	head->env = set_env(envp);
	head->home = ft_strdup(get_var_value(head, "HOME"));
	if (!head->env || !head->home)
	{
		if (head->env)
			free_env(head->env);
		if (head->home)
			free(head->home);
		free(head);
		return (NULL);
	}
	head->tokens = NULL;
	head->pids = NULL;
	head->last_status = 0;
	return (head);
}

int	main(int ac, char **av, char **envp)
{
	t_ms	*head;

	if (ac != 1)
		return (-1);
	(void)av;
	head = send_head(NULL);
	if (!head)
		head = init_head(envp);
	if (!head)
		return (-1);
	sig_control(1);
	while (1)
	{
		if (!head->tokens && !lexer(head))
			continue ;
		// display_tokens(head->tokens);
		if (!is_handle_error(head))
			command_manager(head);
		tokens_clear(head->tokens);
		head->tokens = NULL;
	}
	free_env(head->env);
	free(head);
	return (0);
}

void	display_tokens(t_token *tokens)
{
	t_token	*tmp;
	int		i;
	int		k;

	tmp = tokens;
	i = 0;
	while (tmp)
	{
		printf("\n");
		printf("token #%d", i++);
		if (tmp->type == _none)
			printf("\ntype : _none");
		else if (tmp->type == _cmd_grp)
			printf("\ntype : _cmd_grp");
		else if (tmp->type == _env_var)
			printf("\ntype : _env_var");
		else if (tmp->type == _pipe)
			printf("\ntype : _pipe");
		else if (tmp->type == _redirection)
			printf("\ntype : _redirection");
		else if (tmp->type == _delimiter)
			printf("\ntype : _delimiter");
		else if (tmp->type == _append)
			printf("\ntype : _append");
		else if (tmp->type == _file)
			printf("\ntype : _file");
		printf("\n");
		k = 0;
		while (tmp->value[k])
		{
			printf("value : %s\n", tmp->value[k]);
			++k;
		}
		tmp = tmp->next;
	}
	printf("\n");
	printf("Execution part:\n");
	printf("---------------\n");
	printf("\n");
}
