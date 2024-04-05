#include "minishell.h"

static char	*make_prompt_line(char *username, char *pwd);
static char	*get_username(t_env *env);

char	*prompt(t_env *env)
{
	char	*pwd;
	char	*absolute;
    char    *username;
	char	*line;
	int		i;

	pwd = get_pwd(1);
	if (!pwd)
		return (NULL);
	absolute = get_var_value(env, "HOME");
	if (!absolute)
	{
		free(pwd);
		return (NULL);
	}
	i = 0;
	while (absolute[i])
		++i;
    username = get_username(env);
	absolute = make_prompt_line(username, &pwd[i]);
	free(pwd);
	if (!absolute)
		return (NULL);
	line = readline(absolute);
	return (line);
}

static char	*make_prompt_line(char *username, char *pwd)
{
	char	*user_part;
	char	*prog_name_part;
	char	*pwd_part;
	char	*reset_part;

	user_part = ft_strjoin("\001\033[1;32m\002", username);
	if (!user_part)
		return (NULL);
	prog_name_part = ft_strjoin(user_part, "\001\033[1;0m\033[1;0m@\033[1;36m\002minishell\001\033[1;0m:\033[1;33m\002~");
	free(user_part);
	if (!prog_name_part)
		return (NULL);
	pwd_part = ft_strjoin(prog_name_part, pwd);
	free(prog_name_part);
	if (!pwd_part)
		return (NULL);
	reset_part = ft_strjoin(pwd_part, "\001\033[1;0m\002$ ");
	free(pwd_part);
	if (!reset_part)
		return (NULL);
	return (reset_part);
}

static char	*get_username(t_env *env)
{
    char	*username;

    username = get_var_value(env, "USER");
    if (!username)
        return ("anonynous");
    return (username);
}