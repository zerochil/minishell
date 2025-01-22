NAME = minishell
CC = cc
LIBFT_INCLUDE = ./libft
INCLUDE = ./includes
CFLAGS = -Werror -Wextra -Wall -I$(LIBFT_INCLUDE) -I$(INCLUDE) 

PARAM_EXPANSION_SRC = parameter_expansion.c       \
					  parameter_expansion_utils.c

FIELD_SPLITTING_SRC = field.c       \
					  field_peek.c  \
					  field_utils.c

PATHNAME_EXPANSION_SRC = pattern_ops.c              \
						 pathname_expansion.c       \
						 pathname_expansion_utils.c \

EXPANSION_SRC = $(addprefix parameter_expansion/, $(PARAM_EXPANSION_SRC))   \
				$(addprefix field_splitting/, $(FIELD_SPLITTING_SRC))       \
			   	$(addprefix pathname_expansion/, $(PATHNAME_EXPANSION_SRC)) \
				quote_removal.c           \
				expansion_utils.c         \
				expansion.c

SRCS = minishell.c tokenizer.c tokenizer_input_validator.c lexer.c lexer_functions.c ast.c execution.c builtins.c env.c utils.c prompt.c context.c signals.c \
	   here_document.c here_document_utils.c       \
	   $(addprefix expansion/, $(EXPANSION_SRC)) \
	   execution_pipeline_utils.c stream.c execution_redirection.c execution_simple_command.c execution_simple_command_utils.c execution_utils.c execution_pipeline.c  \
	   debug.c

OBJS_DIR = .objects/
OBJS = $(SRCS:%.c=$(OBJS_DIR)%.o)
HEADER_FILES = minishell.h            \
			   token.h                \
			   lexer.h                \
			   tokenizer.h            \
			   here_document.h        \
			   ast.h                  \
			   expansion.h            \
			   field.h                \
			   execution.h            \
			   execution_pipeline.h   \
			   env.h                  \
			   context.h              \
			   builtins.h             \
			   signals.h              \
			   stream.h               \
			   debug.h                \
			   utils.h

HEADERS = $(HEADER_FILES:%=$(INCLUDE)/%)
LIBFT = libft/libft.a


all: $(NAME)

$(NAME): $(LIBFT) $(OBJS_DIR) $(OBJS)
	$(CC) $(CFLAGS) $(LIBFT) $(OBJS) -o $(NAME) -Llibft -lft -lreadline

$(OBJS_DIR):
	mkdir -p $(OBJS_DIR)

$(OBJS_DIR)%.o: %.c $(HEADERS)
	@mkdir -p $(dir $@)
	$(CC)  $(CFLAGS) -c $< -o $@ 

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
