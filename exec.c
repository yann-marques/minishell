#include "minishell.h"

int is_builtin(char *cmd)
{
    if (strcmp(cmd, "echo"))
        return (1);
    if (strcmp(cmd, "cd"))
        return (1);
    if (strcmp(cmd, "pwd"))
        return (1);
    if (strcmp(cmd, "export"))
        return (1);
    if (strcmp(cmd, "unset"))
        return (1);
    if (strcmp(cmd, "env"))
        return (1);
    if (strcmp(cmd, "exit"))
        return (1);
    return (0);
}

int token_len(t_token *tokens)
{
    int i;

    i = 0;
    while (tokens->next)
        i++;
    return (i);
}

char *find_path(t_token *token, char **envp)
{
    char    **paths;
    char    *path;
    char    *path_cmd;
    int     i;

    if (access(token->value[0], F_OK))
    {
        return (token->value[0]);
    }
    i = 0;
    while (envp[i] && strcmp(envp[i], "PATH") != 0)
        i++;
    if (!envp[i])
        return (NULL);
    paths = ms_split(envp[i], ":");
    while (paths[i])
    {
        path = ms_join_three(paths[i], NULL, "/");
        path_cmd = ms_join_three(path, NULL, token->value[0]);
        free(path);
        if (access(path_cmd, X_OK))
            return (path_cmd);
        free(path_cmd);
        i++;
    }
    return (NULL);
}

int execute(t_token *token, char **envp)
{
    char *path;

    path = find_path(token, envp);
    if (!path)
        return (-1);
    if (execve(path, token->value, envp) == -1)
    {
        perror("error");
        return (-1);
    }
    return (0);
}

void    error_exit(char *str)
{
    printf("%s", str);
    exit(EXIT_FAILURE);
}

void    launch_process(t_token *tokens, char **envp)
{
    int     pid;
    int     fd[2];

    if (pipe(fd) == -1)
        error_exit("Error with the pipe");
    pid = fork();
    if (pid == -1)
        error_exit("Error with the pipe");
    if (pid == 0)
    {
        close(fd[0]);
        dup2(fd[1], STDOUT_FILENO);
        close(fd[1]);
        execute(tokens, envp);
    }
    else
    {
        waitpid(pid, NULL, 0);
        close(fd[1]);
        dup2(fd[0], STDIN_FILENO);
    }
}

int multi_commands(t_token *tokens, char **envp)
{
    t_token *tmp;

    tmp = tokens;
    while (tmp)
    {
        if (tmp->type == _cmd_grp)
            launch_process(tokens, envp);
        tmp = tmp->next;
    }
    return (0);
}

void    command_manager(t_ms *head, char **envp)
{
    t_token *tokens;
    int saved_stdout;

    saved_stdout = dup(STDOUT_FILENO);
    tokens = head->tokens;
    multi_commands(tokens, envp);
    dup2(saved_stdout, STDOUT_FILENO);
}