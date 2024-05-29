NAME=minishell
CC=cc
FLAGS=-Wall -Werror -Wextra -g
FT_LIBFT=srcs/libft_functions/ft_split.c srcs/libft_functions/ft_atoi.c srcs/libft_functions/ft_isalpha.c \
	srcs/libft_functions/ft_isascii.c srcs/libft_functions/ft_isdigit.c srcs/libft_functions/ft_isprint.c \
	srcs/libft_functions/ft_strdup.c srcs/libft_functions/ft_strjoin.c srcs/libft_functions/ft_strlcat.c \
	srcs/libft_functions/ft_strlcpy.c srcs/libft_functions/ft_strlen.c srcs/libft_functions/ft_calloc.c \
	srcs/libft_functions/ft_bzero.c srcs/libft_functions/ft_strncmp.c srcs/libft_functions/ft_itoa.c \
	srcs/libft_functions/ft_strchr.c srcs/gnl/get_next_line.c srcs/gnl/get_next_line_utils.c

BUILTINS=srcs/builtins/ms_cd.c srcs/builtins/ms_echo_n.c srcs/builtins/ms_env.c srcs/builtins/ms_exit.c \
	srcs/builtins/ms_export.c srcs/builtins/ms_pwd.c srcs/builtins/ms_unset.c

EXECS=srcs/exec/exec.c srcs/exec/do_exec.c srcs/exec/pipes_utils.c srcs/exec/commands_utils.c srcs/exec/files/files_utils.c \
	srcs/exec/files/get_paths.c srcs/exec/parse_utils.c srcs/exec/redirect/redirect_utils.c srcs/exec/redirect/do_redirect.c \
	srcs/exec/redirect/here_doc/here_doc.c srcs/exec/redirect/here_doc/here_doc_utils.c srcs/exec/redirect/move_rdout.c srcs/exec/builtins_utils.c

SRC=$(FT_LIBFT) $(BUILTINS) $(EXECS) srcs/main.c srcs/signal.c srcs/lexer.c srcs/env_utils.c srcs/clear.c srcs/lexer_utils.c srcs/ms_split.c srcs/prompt.c \
	srcs/exit.c srcs/error.c srcs/set_type.c srcs/utils.c srcs/var_to_value.c srcs/is_cmd_type_next.c srcs/is_token_type.c srcs/ms_join_three.c

OBJ=$(SRC:.c=.o)
HEADER=srcs/minishell.h srcs/gnl/get_next_line.h

all: $(NAME)

$(NAME): $(OBJ)
	@$(CC) $(FLAGS) $^ -o $@ -lreadline
	@echo "\n\nMinishell created"

%.o: %.c $(HEADER)
	@$(CC) $(FLAGS) -c $< -o $@
	@echo -n '.'

clean:
	@rm -f $(OBJ)
	@echo "All objects clean"

fclean: clean
	@rm -f $(NAME)
	@echo "And the executable too"

re: fclean all

.PHONY: all fclean clean re
