NAME=minishell
CC=gcc
FLAGS=-Wall -Werror -Wextra -g
FT_LIBFT=libft_functions/ft_split.c libft_functions/ft_atoi.c libft_functions/ft_isalpha.c \
	libft_functions/ft_isascii.c libft_functions/ft_isdigit.c libft_functions/ft_isprint.c \
	libft_functions/ft_strdup.c libft_functions/ft_strjoin.c libft_functions/ft_strlcat.c \
	libft_functions/ft_strlcpy.c libft_functions/ft_strlen.c libft_functions/ft_calloc.c \
	libft_functions/ft_bzero.c
BUILTINS=builtins/ms_cd.c builtins/ms_echo_n.c builtins/ms_env.c builtins/ms_exit.c \
	builtins/ms_export.c builtins/ms_pwd.c builtins/ms_unset.c 
SRC=main.c lexer.c env_utils.c exec.c clear.c lexer_utils.c ms_split.c prompt.c \
	set_type.c utils.c var_to_value.c $(FT_LIBFT) $(BUILTINS)
OBJ=$(SRC:.c=.o)
HEADER=minishell.h

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(FLAGS) $^ -o $@ -lreadline

%.o: %.c $(HEADER)
	$(CC) $(FLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)
	rm -f *.gch

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all fclean clean re