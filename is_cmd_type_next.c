#include "minishell.h"

int	is_cmd_rdout(t_token *tk)
{
	if (is_cmd(tk) && tk->next && is_rdout(tk->next))
		return (1);
	return (0);
}

int	is_cmd_rdin(t_token *tk)
{
	if (is_cmd(tk) && tk->next && is_rdin(tk->next))
		return (1);
	return (0);
}

int	is_cmd_heredoc(t_token *tk)
{
	if (is_cmd(tk) && tk->next && is_heredoc(tk->next))
		return (1);
	return (0);
}