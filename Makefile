NAME=minishell
CC=gcc
FLAGS=-Wall -Werror -Wextra -g
SRC=main.c lexer.c env_utils.c builtin.c exec.c libft_functions/ft_split.c \
	libft_functions/ft_atoi.c libft_functions/ft_isascii.c libft_functions/ft_isdigit.c \
	libft_functions/ft_isprint.c libft_functions/ft_strdup.c libft_functions/ft_strjoin.c \
	libft_functions/ft_strlcat.c libft_functions/ft_strlcpy.c libft_functions/ft_strlen.c \
	libft_functions/ft_calloc.c libft_functions/ft_bzero.c
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