NAME = tokenizer_tester
CFLAGS = #-Werror -Wextra -Wall
CC = cc
SRCS = tester.c ../tokenizer.c ../tokenizer_input_validator.c ../lexer.c ../lexer_functions.c
OBJS_DIR = .objects/
OBJS = $(SRCS:%.c=$(OBJS_DIR)%.o)
HEADERS = ../tokenizer.h ../lexer.h
LIBFT = ../libft/libft.a


all: $(NAME)

$(NAME): $(LIBFT) $(OBJS_DIR) $(OBJS)
	$(CC) $(CFLAGS) $(LIBFT) $(OBJS) -o $(NAME) -L../libft -lft -lreadline

$(OBJS_DIR):
	mkdir -p $(OBJS_DIR)

$(OBJS_DIR)%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT): FORCE
	@make -C ../libft --no-print-directory

FORCE:

clean:
	rm -rf $(OBJS_DIR)
	@make -C ../libft clean --no-print-directory

fclean: clean
	rm -rf $(NAME)
	@make -C ../libft fclean --no-print-directory

re: fclean all

.PHONY: all clean fclean re FORCE
