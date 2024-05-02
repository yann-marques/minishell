#include "minishell.h"
#include "gnl/get_next_line.h"

char	*get_token_name(t_token *token)
{
	t_token	*tmp;
	char	*token_name;

	tmp = token;
	token_name = tmp->value[0];
	if (is_rdout(tmp) && !tmp->value[1] && tmp->next
		&& is_rdout(tmp->next) && !tmp->next->value[1])
		token_name = ">";
	if (tmp->type == _delimiter && !tmp->value[1])
		token_name = "newline";
	if ((is_rdin(tmp) || is_rdout(tmp)) && !tmp->value[1]
		&& !tmp->next && (!tmp->prev || tmp->prev->type == _pipe))
		token_name = "newline";
	if ((is_rdin(tmp) || is_rdout(tmp)) && !tmp->value[1]
		&& tmp->next && tmp->next->type == _pipe)
		token_name = "|";
	if (is_rdin(tmp) && !tmp->value[1] && tmp->prev && !tmp->next)
		token_name = "newline";
	return (token_name);
}

static void	do_case_error(t_token **tmp, char **token_name, int *i)
{
	if (is_heredoc(*tmp) && !(*tmp)->value[1])
		*token_name = get_token_name(*tmp);
	if (is_heredoc(*tmp))
		*token_name = get_token_name(*tmp);
	if (*i == 0 && (*tmp)->type == _pipe)
	{
		*token_name = "|";
		*tmp = NULL;
		return ;
	}
	if ((*tmp)->type == _pipe && !(*tmp)->next)
		*token_name = "|";
	if ((is_rdin(*tmp) || is_rdout(*tmp)) && (!(*tmp)->value[1]
			|| (*tmp)->value[1][0] == '>' || (*tmp)->value[1][0] == '<'))
		*token_name = get_token_name(*tmp);
	*tmp = (*tmp)->next;
	*i = *i + 1;
	return ;
}

int	is_handle_error(t_ms *head)
{
	t_token	*tmp;
	char	*tk_name;
	char	*error_line;
	int		i;

	if (!head->tokens)
		return (1);
	tmp = head->tokens;
	i = 0;
	tk_name = NULL;
	while (tmp)
		do_case_error(&tmp, &tk_name, &i);
	if (!tk_name)
		return (0);
	tk_name = ft_strjoin(" syntax error near unexpected token `", tk_name);
	if (!tk_name)
		error_exit(" ", -1);
	error_line = ft_strjoin(tk_name, "'\n");
	if (!error_line)
		error_exit(" ", -1);
	write (2, error_line, ft_strlen(error_line));
	head->last_status = 2;
	free(error_line);
	return (1);
}

int	perror_str(char *str, int status)
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
