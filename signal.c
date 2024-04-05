#include "minishell.h"

static void	child_sig(int signum);
static void	parent_sig(int signum);

void	sig_control(int	child)
{
	if (child)
	{
		signal(SIGINT, child_sig);
		signal(SIGQUIT, child_sig);
	}
	else
	{
		signal(SIGINT, parent_sig);
		signal(SIGQUIT, parent_sig);
	}
}

static void	child_sig(int signum)
{
	if (signum == SIGINT)
	{
		write(1, "\n", 1);
		rl_on_new_line(); 
		rl_redisplay();
	}
	else
		ms_exit(NULL);
}

static void	parent_sig(int signum)
{
	if (signum == SIGINT)
	{
		write(1, "\n", 1);
		rl_on_new_line(); 
		rl_redisplay();
	}
	else
		ms_exit(NULL);
}
