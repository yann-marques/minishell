#include "minishell.h"
#include "gnl/get_next_line.h"

int	print_error_message_token(t_ms *head, t_token *token)
{
	char	*error_line;
	char	*token_name;
	int		i;
	t_token *tmp;


	if (!token)
		return (1);
	tmp = token;
	i = 0;
	while (tmp->value[i])
		i++;
	if (i != 0)
		i--;
	token_name = ft_strjoin(tmp->value[i], "'\n");
	if (!token_name)
		perror_exit("", -1);
	if (tmp->type == _delimiter && !tmp->value[1])
		token_name = ft_strdup("newline'\n");
	if ((is_rdin(tmp) || is_rdout(tmp)) && !tmp->value[1])
		token_name = ft_strdup("newline'\n");
	if ((is_rdin(tmp) || is_rdout(tmp)) && !tmp->value[1] && tmp->prev && tmp->prev->type == _redirection)
		token_name = ft_strdup(">'\n");	
	error_line = ft_strjoin(" syntax error near unexpected token `", token_name);
	free(token_name);
	if (!error_line)
		perror_exit("", -1);
	write (2, error_line, ft_strlen(error_line));
	free(error_line);
	head->last_status = 2;
	return (1);
}

int is_handle_eror(t_ms *head)
{
	t_token *tmp;
	int		i;

	if (!head->tokens)
		return (1);
	tmp = head->tokens;
	while (tmp->next)
		tmp = tmp->next;
	i = 0;
	while(tmp)
	{
		printf("\n[%s]\n", tmp->value[0]);
		if (is_heredoc(tmp) && !tmp->value[1])
			return (print_error_message_token(head, tmp));
		if (is_heredoc(tmp))
			return (0);
		if (i == 0 && tmp->type == _pipe)
			return (print_error_message_token(head, tmp));
		if ((is_rdin(tmp) || is_rdout(tmp)) && (!tmp->value[1] || tmp->value[1][0] == '>' || tmp->value[1][0] == '<'))
			return (print_error_message_token(head, tmp));
		tmp = tmp->prev;
		i++;
	}
	return (0);
}


int perror_str(char *str, int status)
{
	if (str)
	{
		perror(str);
		return (status);
	}
	perror("");
	return (status);
}

int	error_str(char *str)
{
	if (str)
		write(2, str, ft_strlen(str));
	return (EXIT_FAILURE);
}