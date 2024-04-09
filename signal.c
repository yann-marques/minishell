#include "minishell.h"

static void	handle_fork(int signum);
static void	handler_sigint(int signum);

int	sig_received;

void	sig_control(int handle)
{
	if (handle)
	{
		signal(SIGINT, handler_sigint);
		signal(SIGQUIT, SIG_IGN);
	}
	else
	{
		signal(SIGINT, handle_fork);
		signal(SIGQUIT, handle_fork);
	}
}

static void	handle_fork(int signum)
{
	sig_received = signum;
	printf("\n");
}

static void	handler_sigint(int signum)
{
	(void)signum;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_redisplay();
}
