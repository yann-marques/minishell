NAME=minishell
CC=gcc
FLAGS=-Wall -Werror -Wextra -g
SRC=main.c lexer.c env_utils.c
OBJ=$(SRC:.c=.o)
HEADER=minishell.h

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(FLAGS) $^ -o $@ -lreadline

%.o: %.c $(HEADER)
	$(CC) $(FLAGS) -c $<

clean:
	rm -f $(OBJ)
	rm -f *.gch

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all fclean clean re