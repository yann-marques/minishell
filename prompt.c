/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymarques <ymarques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 17:15:21 by yanolive          #+#    #+#             */
/*   Updated: 2024/05/17 15:17:39 by ymarques         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*make_prompt_line(char *username, char *pwd);
static char	*get_username(t_ms *head);

char	*prompt(t_ms *head)
{
	char	*pwd;
	char	*absolute;
	char	*username;
	char	*line;

	pwd = get_pwd(1);
	if (!pwd)
		return (NULL);
	absolute = ft_strdup(get_var_value(head, "HOME"));
	if (!absolute)
		absolute = ft_strdup(head->home);
	if (!absolute)
	{
		free(pwd);
		return (NULL);
	}
	username = get_username(head);
	line = make_prompt_line(username, &pwd[ft_strlen(absolute)]);
	free(pwd);
	free(absolute);
	return (line);
}

char	*line_error(char *prompt_line, t_ms *head)
{
	char	*line;

	line = readline(prompt_line);
	if (!line)
	{
		free(prompt_line);
		ms_exit(head, NULL);
	}
	if (!line[0])
	{
		free(line);
		line = line_error(prompt_line, head);
		return (line);
	}
	add_history(line);
	free(prompt_line);
	return (line);
}

static char	*make_prompt_line(char *username, char *pwd)
{
	char	*user_part;
	char	*prog_name_part;
	char	*pwd_part;
	char	*reset_part;
	char	*p;

	user_part = ft_strjoin("\001\033[1;32m\002", username);
	if (!user_part)
		return (NULL);
	p = "\1\033[1;0m\033[1;0m@\033[1;36mminishell\2\1\033[1;0m:\033[1;33m\2~";
	prog_name_part = ft_strjoin(user_part, p);
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

static char	*get_username(t_ms *head)
{
	char	*username;

	username = get_var_value(head, "USER");
	if (!username)
		return ("anonynous");
	return (username);
}
