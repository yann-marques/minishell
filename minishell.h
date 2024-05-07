#ifndef MINISHELL_H
# define MINISHELL_H

// Not interpret : " \ ;

// Pour readline, rl_clear_history, rl_on_new_line, rl_replace_line, rl_redisplay, add_history
# include <readline/readline.h>
# include <readline/history.h>
// Pour printf, perror
# include <stdio.h>
// Pour malloc, free, exit
# include <stdlib.h>
// Pour write, access, open, read, close, fork, wait, waitpid, wait3, wait4, isatty, ttyname, ttyslot, ioctl
# include <unistd.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/wait.h>
// Pour signal, sigaction, sigemptyset, sigaddset, kill
# include <signal.h>
// Pour getcwd, chdir, stat, lstat, fstat, unlink, execve
# include <sys/stat.h>
// Pour dup, dup2, pipe
# include <unistd.h>
// Pour opendir, readdir, closedir
# include <dirent.h>
// Pour strerror, errno
# include <string.h>
# include <errno.h>
// Pour getenv, tcsetattr, tcgetattr, tgetent, tgetflag, tgetnum, tgetstr, tgoto, tputs
// # include <term.h> // probleme avec sa !
# include <termios.h>
# include <curses.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 100
# endif

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

typedef struct s_ms
{
	t_token	*tokens;
	int		token_count;
	t_env	*env;
	t_pids	*pids;
	int		last_status;
	char	*HOME;
	char	*USER;
}	t_ms;

extern int	g_sig_received;

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
char	*get_username(t_ms *head);
void	set_type(t_token *tokens);
char	**var_to_value(char **tab, t_ms *head);
int		check_if_replace(char *str, int i_var);
//lexer_utils
int		del_quotes(t_token *tokens);
int		tokens_addback(t_token **tokens, t_type type, char **value);
int		quotes_jump(char *str);
int		check_quotes(char *str);
int		ft_strtab_len(char **tab);
//ms_split
char	**ms_split(char *str, char *sep);
//builtins
int		ms_cd(t_token *token);
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
void	tokens_clear(t_token *tokens);
void	pids_clear(t_pids *pids);
void	free_env(t_env *env);

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
int		redirection_in(t_token *token);
int		do_redirection_in(t_ms *head, t_token *token);
char	*here_doc(t_token *token);
void	rd_null(void);
int		check_file_out(t_token *token);

//EXEC: parse utils
char	**t_env_to_strtab(t_env *env);
t_token	*get_n_token(t_token *tokens, int count);
int		env_size(t_env *env);

//EXEC: files utils
char	*get_random_tmp_path(void);
int		do_needed_files(t_token *token);
int		is_file_error_in_pipe(t_token *token);
int		is_file(char *name);

//EXEC: exec.c
int		multi_commands(t_ms *head);
void	command_manager(t_ms *head);
int		set_tk_at_next_cmd(t_token *token);

//EXEC: do_exec.c
int		do_pipe_error(t_ms *head, t_token **token);
int		do_heredoc(t_ms *head, t_token **token, char **path_doc);
int		do_cmd_and_rd(t_ms *head, t_token **tk, char *path_doc);
int		do_rd(t_ms *head, t_token **tk);

//EXEC: commands utils
char	*find_path(t_ms *head, t_token *token);
int		execute(t_ms *head, t_token *token);
int		pipe_and_exec(t_ms *head, t_token *token,
			char *path_doc, int last_command);

//EXEC: builtins utils
int		check_echo_builtin(char **value);
int		builtin_child(t_ms *head, t_token *token);
int		check_if_builtins_parent(t_ms *head, t_token *token);

//EXEC: pipes utils
t_token	*get_next_pipe(t_token *tk);
int		have_next_pipe(t_token *token);

//exit
void	error_exit(char *str, int status);
void	perror_exit(char *str, int status);

//error
int		error_str(char *str);
int		perror_str(char *str, int status);
int		is_handle_error(t_ms *head);

t_ms	*send_head(t_ms *new_head);

void	display_tokens(t_token *tokens);

#endif