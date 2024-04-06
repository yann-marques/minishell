#include "minishell.h"

static void	handler_sigint(int signum);

void	sig_control(void)
{
	signal(SIGINT, handler_sigint);
	signal(SIGQUIT, SIG_IGN);
}

static void	handler_sigint(int signum)
{
	(void)signum;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_redisplay();
}
