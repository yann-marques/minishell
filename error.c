#include "minishell.h"
#include "gnl/get_next_line.h"

void	print_error_message_token(t_token *token)
{
	char	*error_line;
	char	*token_name;
	int		i;
	t_token *tmp;


	if (!token)
		return ;
	tmp = token;
	i = 0;
	while (tmp->value[i])
		i++;
	if (i != 0)
		i--;
	token_name = ft_strjoin(tmp->value[i], "'\n");
	if (!token_name)
		perror_exit("", -1);
	if (token->type == _delimiter || (token->type == _append && !tmp->value[0][2]))
		token_name = ft_strdup("newline'\n");
	if (token->type == _append && tmp->value[0][2])
		token_name = ft_strdup(">'\n");
	if (token->type == _redirection && !tmp->value[1] && !tmp->value[i][1])
		token_name = ft_strdup("newline'\n");
	error_line = ft_strjoin(" syntax error near unexpected token `", token_name);
	free(token_name);
	if (!error_line)
		perror_exit("", -1);
	write (2, error_line, ft_strlen(error_line));
	free(error_line);
}

int is_handle_eror(t_ms *head)
{
	t_token *tmp;
	int		i;

	if (!head->tokens)
		return (1);
	tmp = head->tokens;
	i = 0;
	while(tmp)
	{
		if (i == 0 && !is_cmd(tmp))
		{
			head->last_status = 2;
			print_error_message_token(tmp);
			return (1);
		}
		if (tmp->type == _pipe && tmp->next && is_rdout(tmp->next))
		{
			if (tmp->next->value[1] && tmp->next->value[1][0] != '>')
			{
				tmp = tmp->next;
				continue;
				i++;
			}
			head->last_status = 2;
			print_error_message_token(tmp->next);
			return (1);
		}
		if (is_rdin(tmp) && !tmp->value[1] && tmp->next)
		{
			head->last_status = 2;
			print_error_message_token(tmp->next);
			return (1);
		}
		if (is_rdin(tmp) && !tmp->value[1])
		{
			head->last_status = 2;
			print_error_message_token(tmp);
			return (1);
		}
		if (!is_cmd(tmp) && (!is_rdin(tmp) && !is_rdout(tmp)) && !tmp->next)
		{
			head->last_status = 2;
			print_error_message_token(tmp);
			return (1);
		}
		tmp = tmp->next;
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