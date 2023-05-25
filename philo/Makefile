NAME := philo
FLAGS := -Wall -Wextra -Werror -Ofast

SRCS := $(addprefix src/,\
	philosophers.c \
	routine.c \
	eating.c \
	dying.c \
	utils.c \
)

.PHONY: all bonus clean fclean re

all: $(NAME)

$(NAME):
	@echo "Compiling $(NAME)..."
	@cc $(FLAGS) $(SRCS) -o $(NAME) -Ofast

clean:

fclean: clean
	@rm -f $(NAME)

re: fclean all
