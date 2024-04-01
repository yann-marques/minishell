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

typedef struct s_token
{
	t_type			type;
	char			**value;
	struct s_token	*next;
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
}	t_ms;



//LIBFT PART
char	**ft_split(char const *s, char c);
char	*ft_strjoin(char const *s1, char const *s2);
int	ft_atoi(const char *nptr);
int	ft_isascii(int c);
int	ft_isprint(int c);
int	ft_isdigit(int c);
char	*ft_strdup(const char *s);
char	*ft_strjoin(char const *s1, char const *s2);
size_t	ft_strlcat(char *dst, const char *src, size_t size);
size_t	ft_strlcpy(char *dst, const char *src, size_t size);
size_t	ft_strlen(const char *s);
void	*ft_calloc(size_t nmemb, size_t size);
void	ft_bzero(void *s, size_t n);



// lexer
t_ms	*lexer(t_env *env);
int		quotes_jump(char *str);
t_token	*set_tokens(char *str);
void	set_type(t_token *tokens);
t_token	*redirection_type(t_token *token);
char	*ms_join_three(char *s1, char *s2, char *s3);
// builtin
int		ms_echo_n(t_token *token);
int		ms_cd(t_token *token);
int		ms_pwd(void);
char	*get_pwd(int count);
int		ms_export(t_env	*env, t_token *token);
int		ms_unset(t_env *env, t_token *token);
int		ms_env(t_env *env, char *begin);
int		ms_exit(t_ms *head);
// t_token utils
void	tokens_clear(t_token *tokens);
int		tokens_addback(t_token **tokens, t_type type, char **value);
// env_var
t_env	*set_env(char **env);
char	*get_var_value(t_env *env, char *var);
t_env	*env_new(char *env);
void	put_env_var(t_env **env, t_env *new);
void	env_addback(t_env **env, t_env *new);
void	free_env(t_env *env);
// basic utils
void	strtab_clear(char **tab);
char	*ft_strchr(char *str, char c);
char	*ft_strndup(char *str, int n);
int		ft_strlen_to(char *str, char c);
int		ft_strcmp(char *s1, char *s2);
// ms_split
char	**ms_split(char *str, char *sep);
int		ms_split_size(char *str, char *sep);
int		get_next_sep(char *str, char *sep);
// checks
int		check_quotes(char *str);
int		check_env_var(t_token *begin_tokens, t_token *token, char *str);
int		ft_isalpha(char c);
//exec
void	command_manager(t_ms *head);
t_ms	*send_head(t_ms *new_head);

#endif
