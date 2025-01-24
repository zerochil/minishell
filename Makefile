NAME = minishell
CC = cc
LIBFT_INCLUDE = ./libft
INCLUDE = ./includes
CFLAGS = -Werror -Wextra -Wall -I$(LIBFT_INCLUDE) -I$(INCLUDE) #-fsanitize=address

LEXER_SRC = lexer.c           \
			lexer_functions.c \

TOKENIZER_SRC = tokenizer.c            \
			   tokenizer_utils.c      \
			   tokenizer_operations.c \

AST_SRC = ast.c            \
		  ast_utils.c      \
		  ast_build.c      \
		  ast_errors.c     \
		  ast_predicates.c \

HERE_DOC_SRC = here_document.c       \
			   here_document_utils.c \

PARAM_EXPANSION_SRC = parameter_expansion.c       \
					  parameter_expansion_utils.c \

FIELD_SPLITTING_SRC = field.c       \
					  field_peek.c  \
					  field_utils.c \

PATHNAME_EXPANSION_SRC = pattern_ops.c              \
						 pathname_expansion.c       \
						 pathname_expansion_utils.c \

PARSING_SRC = $(addprefix lexer/, $(LEXER_SRC))            \
			  $(addprefix tokenizer/, $(TOKENIZER_SRC))    \
			  $(addprefix ast/, $(AST_SRC))                \
			  $(addprefix here_document/, $(HERE_DOC_SRC)) \

EXPANSION_SRC = $(addprefix parameter_expansion/, $(PARAM_EXPANSION_SRC))   \
				$(addprefix field_splitting/, $(FIELD_SPLITTING_SRC))       \
			   	$(addprefix pathname_expansion/, $(PATHNAME_EXPANSION_SRC)) \
				quote_removal.c                                             \
				expansion_utils.c                                           \
				expansion.c

EXECUTION_SRC = execution.c                       \
				execution_pipeline.c              \
				execution_predicates.c            \
				execution_redirection.c           \
				execution_pipeline_utils.c        \
				execution_simple_command.c        \
				execution_simple_command_utils.c  \
				stream.c                          \

BUILTIN_SRC = builtins.c             \
			  builtins_utils.c       \
			  builtins_environment.c \

ENV_SRC = env.c            \
		  env_init.c       \
		  env_utils.c      \
		  env_predicates.c \

SRCS = minishell.c context.c context_get_set.c   \
	   utils.c debug.c signals.c                 \
	   $(addprefix parsing/, $(PARSING_SRC))     \
	   $(addprefix expansion/, $(EXPANSION_SRC)) \
	   $(addprefix execution/, $(EXECUTION_SRC)) \
	   $(addprefix builtins/, $(BUILTIN_SRC))    \
	   $(addprefix env/, $(ENV_SRC))             \

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
	$(CC)  $(CFLAGS) $(LIBFT) $(OBJS) -o $(NAME) -Ilibft -Llibft -lft -lreadline

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
