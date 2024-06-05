/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymarques <ymarques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 17:10:26 by yanolive          #+#    #+#             */
/*   Updated: 2024/06/05 13:44:05 by ymarques         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <readline/readline.h>
# include <readline/history.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <signal.h>
# include <sys/stat.h>
# include <unistd.h>
# include <dirent.h>
# include <string.h>
# include <errno.h>
# include <termios.h>
# include <curses.h>
# include <stdint.h>
# include <limits.h>
# include <sys/ioctl.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 10
# endif

extern int					g_sig_received;

typedef struct __dirstream	t_dir;

typedef enum e_type
{
	_none,
	_cmd_grp,
	_env_var,
	_pipe,
	_redirection,
	_delimiter,
	_append,
	_file,
}	t_type;

typedef struct s_pids
{
	int				pid;
	struct s_pids	*next;
}	t_pids;

typedef struct s_token
{
	t_type			type;
	char			**value;
	struct s_token	*next;
	struct s_token	*prev;
}	t_token;

typedef struct s_env
{
	char			*var;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_heredoc
{
	char				*line;
	struct s_heredoc	*next;
}	t_heredoc;

typedef struct s_ms
{
	t_token			*tokens;
	t_token			**tokens_cpy;
	int				token_count;
	t_env			*env;
	t_pids			*pids;
	int				last_status;
	int				original_stdint;
	int				heredoc_stdint;
}	t_ms;

//libft_fonctions
char	**ft_split(char const *s, char c);
char	*ft_strjoin(char const *s1, char const *s2);
int		ft_atoi(const char *nptr);
int		ft_isalpha(int c);
int		ft_isascii(int c);
int		ft_isprint(int c);
int		ft_isdigit(int c);
char	*ft_strdup(const char *s);
char	*ft_strjoin(char const *s1, char const *s2);
size_t	ft_strlcat(char *dst, const char *src, size_t size);
size_t	ft_strlcpy(char *dst, const char *src, size_t size);
size_t	ft_strlen(const char *s);
void	*ft_calloc(size_t nmemb, size_t size);
void	ft_bzero(void *s, size_t n);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
char	*ft_itoa(int n);

//signal
void	sig_control(int handle);
//env_utils
t_env	*set_env(char **env);
char	*get_var_value(t_ms *head, char *var);
t_env	*env_new(char *env);
void	put_env_var(t_env **env, t_env *new);
int		pids_addback(t_pids **pids, int pid);
//lexer
int		lexer(t_ms *head);
char	*prompt(t_ms *head);
char	*line_error(char *prompt_line, t_ms *head);
void	set_type(t_token *tokens);
char	**var_to_value(char **tab, t_ms *head);
char	*ms_join_three(char *s1, char *s2, char *s3, int free_var);
int		isin_dblquotes(char *str, int i_target, bool dblquotes);
//lexer_utils
int		del_quotes(t_token *tokens);
int		tokens_addback(t_token **tokens, t_type type, char **value);
int		quotes_jump(char *str);
int		check_quotes(char *str);
int		ft_strtab_len(char **tab);
char	*ft_strdup_noquotes(char *str);
//ms_split
char	**ms_split(char *str, char *sep);
//builtins
int		ms_cd(t_ms *head, t_token *token);
int		ms_echo_n(t_token *token);
int		ms_env(t_env *env, char *begin);
int		ms_exit(t_ms *head, t_token *token);
int		ms_export(t_env	*env, t_token *token);
int		ms_pwd(void);
char	*get_pwd(int count);
int		ms_unset(t_env *env, t_token *token);
//utils
char	*ft_strndup(char *str, int n);
char	*ft_strchr(char *str, char c);
int		ft_strlen_to(char *str, char c);
int		ft_strcmp(char *s1, char *s2);
//clear
void	strtab_clear(char **tab);
void	tokens_clear(t_ms *head);
void	pids_clear(t_pids *pids);
void	free_env(t_env *env);
void	exit_free_head(t_ms *head, int exit);

//is_token_type (BOOL FUNCTIONS)
int		is_cmd(t_token *tk);
int		is_rdin(t_token *tk);
int		is_rdout(t_token *tk);
int		is_heredoc(t_token *tk);

//is_cmd_type_next (BOOL FUNCTIONS)
int		is_cmd_rdout(t_token *tk);
int		is_cmd_rdin(t_token *tk);
int		is_cmd_heredoc(t_token *tk);
int		next_redirect(t_token *tk);
int		next_redirect_out(t_token *tk);

//EXEC: redirect utils
void	redirection_out(int fd_out);
int		do_redirection_out(t_ms *head, t_token *tk);
int		redirection_in(t_ms *head, t_token *token);
int		do_redirection_in(t_ms *head, t_token *token);
int		here_doc(t_ms *head, t_token *token);
int		heredoc_addback(t_heredoc **here_doc, char *line);
void	rd_null(t_ms *head);
int		check_file_out(t_token *token);
void	move_rdout(t_token **tk);
int		free_rest_gnl(int fd, char *line, char *limiter, int return_code);
void	fill_heredoc(char *line, char *lim, t_ms *head, int fd);

//EXEC: parse utils
char	**t_env_to_strtab(t_env *env);
t_token	*get_n_token(t_token *tokens, int count);
int		env_size(t_env *env);
int		heredoc_size(t_heredoc *here_doc);
char	**t_heredoc_to_strtab(t_heredoc *tab);

//EXEC: files utils
char	*get_random_tmp_path(t_ms *head);
int		do_needed_files(t_token *token);
int		is_file_error_in_pipe(t_token *token);
int		is_file(char *name);
int		have_slash(char *path);
char	*get_relative_path(char *path);

//EXEC: exec.c
int		multi_commands(t_ms *head);
void	command_manager(t_ms *head);
int		set_tk_at_next_cmd(t_token **token);
int		set_tk_at_after_stdin(t_token **token);

//EXEC: do_exec.c
int		do_pipe_error(t_ms *head, t_token **token);
int		do_heredoc(t_ms *head, t_token **token);
int		do_cmd_and_rd(t_ms *head, t_token **tk);
int		do_rd(t_ms *head, t_token **tk);

//EXEC: commands utils
char	*find_path(t_ms *head, t_token *token);
int		execute(t_ms *head, t_token *token);
int		pipe_and_exec(t_ms *head, t_token *token, int last_command);

//EXEC: builtins utils
int		check_echo_builtin(char **value);
int		builtin_child(t_ms *head, t_token *token);
int		check_if_builtins_parent(t_ms *head, t_token *token);

//EXEC: pipes utils
t_token	*get_next_pipe(t_token *tk);
int		have_next_pipe(t_token *token);

//exit
void	error_exit(t_ms *head, char *str, int status);
void	perror_exit(t_ms *head, char *str, int status);

//error
int		error_str(char *str);
int		perror_str(char *str, int status);
int		is_handle_error(t_ms *head);

t_ms	*send_head(t_ms *new_head);

#endif