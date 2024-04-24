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
	if (!head->env)
	{
		free(head);
		return (NULL);
	}
	head->tokens = NULL;
	head->pids = NULL;
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
	{
		head = init_head(envp);
		if (!head)
			return (-1);
	}
	sig_control(1);
	while (1)
	{
		if (!head->tokens && !lexer(head))
			continue ;
		// display_tokens(head->tokens);
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
