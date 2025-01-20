NAME = minishell
CFLAGS = -Werror -Wextra -Wall
CC = cc
SRCS = minishell.c tokenizer.c tokenizer_input_validator.c lexer.c lexer_functions.c ast.c expansion.c here_document.c execution.c builtins.c env.c utils.c prompt.c context.c signals.c \
	   execution_pipeline_utils.c stream.c execution_redirection.c execution_simple_command.c execution_simple_command_utils.c execution_utils.c execution_pipeline.c  \
	   debug.c
OBJS_DIR = .objects/
OBJS = $(SRCS:%.c=$(OBJS_DIR)%.o)
HEADERS = minishell.h tokenizer.h lexer.h ast.h expansion.h here_document.h execution.h builtins.h env.h utils.h context.h signals.h execution_pipeline.h stream.h \
		  debug.h
LIBFT = libft/libft.a


all: $(NAME)

$(NAME): $(LIBFT) $(OBJS_DIR) $(OBJS)
	$(CC) $(CFLAGS) $(LIBFT) $(OBJS) -o $(NAME) -Llibft -lft -lreadline

$(OBJS_DIR):
	mkdir -p $(OBJS_DIR)

$(OBJS_DIR)%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT): FORCE
	@make -C libft --no-print-directory

FORCE:

clean:
	rm -rf $(OBJS_DIR)
	@make -C libft clean --no-print-directory

fclean: clean
	rm -rf $(NAME)
	@make -C libft fclean --no-print-directory

re: fclean all

.PHONY: all clean fclean re FORCE
