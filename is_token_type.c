#include "minishell.h"

int	is_cmd(t_token *tk)
{
	if (tk->type == _cmd_grp)
		return (1);
	return (0);
}
int	is_rdin(t_token *tk)
{
	if (tk->value[0][1])
		return (0);
	if (tk->type == _redirection && tk->value[0][0] == '<')
		return (1);
	return (0);
}

int	is_rdout(t_token *tk)
{
	if (tk->type == _redirection && tk->value[0][0] == '>')
		return (1);
	if (tk->type == _append)
		return (1);
	return (0);
}
int	is_heredoc(t_token *tk)
{
	if (tk->value[0][2])
		return (0);
	if (tk->type == _delimiter)
		return (1);
	return (0);
}