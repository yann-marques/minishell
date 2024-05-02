#include "../minishell.h"
#include "../gnl/get_next_line.h"

int	have_next_pipe(t_token *token)
{
	t_token	*tmp;

	if (token->next)
		tmp = token->next;
	else
		return (0);
	while (tmp)
	{
		if (tmp->type == _pipe)
			return (1);
		tmp = tmp->next;
	}
	return (0);
}

t_token	*get_next_pipe(t_token *tk)
{
	t_token	*tmp;

	tmp = tk;
	if (tmp->type == _pipe && tmp->next)
		return (tmp->next);
	while (tmp && tmp->type != _pipe)
		tmp = tmp->next;
	if (tmp && tmp->type == _pipe && tmp->next)
		return (tmp->next);
	return (tmp);
}
