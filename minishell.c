#include "minishell.h"
#include "libft/containers/array/array.h"
#include "libft/containers/string/string.h"
#include "libft/memory_management/memory_management.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <stddef.h>
#include <strings.h>

typedef enum e_ast_type
{
	AST_BINARY,
	AST_BINARY_OR,
	AST_BINARY_AND,
	AST_PIPELINE,
	AST_COMMAND,
	AST_SUBSHELL,
	AST_SIMPLE_COMMAND,
} t_ast_type;

typedef struct s_ast t_ast;
typedef struct s_token t_token;

typedef struct s_node_binary
{
	t_ast_type type;
	t_ast *left;
	t_ast *right;
} t_node_binary;

typedef struct s_node_pipeline
{
	t_array *commands;
} t_node_pipeline;

typedef struct s_subshell
{
	t_ast *list;
	t_array *redirection_list;
}	t_subshell;

struct s_token
{
	int	type;
	union
	{
		char *word;
		char *filename;
	};
};

typedef struct s_simple_command
{
	t_array *tokens;
} t_simple_command;

typedef struct s_node_command
{
	t_ast_type type;
	union
	{
		t_subshell *subshell;
		t_simple_command *simple_command;
	};
} t_node_command;

struct s_ast
{
	t_ast_type type;
	union
	{
		t_node_binary *binary;
		t_node_pipeline *pipeline;
		t_node_command *command;
	};
};





// LEXER
// type
// symbol >>
// identifier "REDIRECIT_APPEND"


typedef struct s_lexem
{
	int type;
	char *symbol;
	char *identifier;
	size_t symbol_length;
	size_t identifier_length;
} t_lexem;


static void lexems_init(t_array*);
t_array *lexems_get_instance();

static void lexem_add(char* symbol, char *identifier)
{
	static int id = 1;
	t_array *lexems;
	t_lexem *lexem;

	lexems = lexems_get_instance();
	lexem = track_malloc(sizeof(t_lexem));
	lexem->type = id;
	lexem->symbol = symbol;
	lexem->identifier = identifier;
	lexem->symbol_length = ft_strlen(symbol);
	lexem->identifier_length = ft_strlen(identifier);
	array_push(lexems, lexem);
	id++;
}

// && || >> << > < ( ) |
static void lexems_init(t_array *lexems)
{
	array_init(lexems);
	lexem_add("&&", "LOGIC_AND");
	lexem_add("||", "LOGIC_OR");
	lexem_add(">>", "REDIRECTION_APPEND");
	lexem_add("<<", "HERE_DOCUMENT");
	lexem_add(">", "REDIRECTION_TRUNC");
	lexem_add("<", "REDIRECTION_IN");
	lexem_add("(", "OPEN_PARENTHESIS");
	lexem_add(")", "CLOSE_PARENTHESIS");
	lexem_add("\n", "NEWLINE");
	lexem_add("|", "PIPE");
	lexem_add("", "EOF");
}

static void lexems_destroyer(void *ptr)
{
	t_array *array;
	
	array = ptr;
	array_destroy(array);
	free(array);
}

t_array *lexems_get_instance()
{
	static t_array *lexems;
	if (lexems == NULL)
	{
		lexems = safe_malloc(sizeof(t_array));
		track_resource(lexems, lexems_destroyer);
		lexems_init(lexems);
	}
	return lexems;
}

static int lexem_match_identifier(void *lexem_ptr, void *identifier)
{
	t_lexem *lexem;

	lexem = lexem_ptr;
	return (ft_strncmp(lexem->identifier, identifier, lexem->identifier_length) == 0);
}

int lexem_get_type(char *identifier)
{
	t_array *lexems;
	t_lexem *lexem;

	lexems = lexems_get_instance();
	lexem = array_find(lexems, identifier, lexem_match_identifier);
	if (lexem == NULL)
		return (0);
	return (lexem->type);
}


// TOKENIZATION

int is_space(char c)
{
	return (c == ' ' || c == '\t' || c == '\v');
}

void skip_whitespace(t_string *input)
{
	while (is_space(string_peek(input)))
		string_shift(input);
}

static int lexem_is_redirection(int type)
{
	return (
		type == lexem_get_type("REDIRECTION_IN") ||
		type == lexem_get_type("REDIRECTION_APPEND") ||
		type == lexem_get_type("REDIRECTION_TRUNC") ||
		type == lexem_get_type("HERE_DOCUMENT")
	);
}

t_token *token_init(int type, char *value)
{
	t_token *token;

	token = track_malloc(sizeof(t_token));
	/*track_transfer_ownership(token, "global");*/
	token->type = type;
	if (lexem_is_redirection(type))
		token->filename = value;
	else if (type == 0)
		token->word = value;
	return token;
}

int	lexem_match_symbol(void *lexem_ptr, void *input_ptr)
{
	t_lexem *lexem;
	t_string *input;

	lexem = lexem_ptr;
	input = input_ptr;
	return string_match(input, lexem->symbol, 0);
}

#define META_CHARACTERS "|&;()<>\n \t"

t_token *tokenize_word(t_string *input)
{
	char	c;
	char	in_quote;
	char	*word;

	in_quote = '\0';
	string_peek_reset(input);
	skip_whitespace(input);
	while(1)
	{
		c = string_peek(input);
		if (in_quote == '\0' && ft_strchr(META_CHARACTERS, c))
			break;
		else if (in_quote == c)
			in_quote = '\0';
		else if (c == '\'' || c == '"')
			in_quote = c;
		string_peek_advance(input);
	}
	word = string_segment_slice(input, 0, input->peek);
	return (token_init(0, word));
}

t_token *tokenize_non_word(t_string *input)
{
	t_array	*lexems;
	t_lexem	*lexem;
	t_token	*token;
	t_token	*next_token;

	lexems = lexems_get_instance();
	token = NULL;
	skip_whitespace(input);
	lexem = array_find(lexems, input, lexem_match_symbol);
	if (lexem == NULL)
		return (NULL);
	string_shift_by(input, lexem->symbol_length);
	if (lexem_is_redirection(lexem->type))
	{
		next_token = tokenize_word(input);
		token = token_init(lexem->type, next_token->word);
	}
	else if (lexem->type != lexem_get_type("EOF"))
		token = token_init(lexem->type, NULL);
	return token;
}

t_token *token_next(t_string *input)
{
	t_token	*token;

	string_peek_reset(input);
	if (string_peek(input) == '\0')
		return (token_init(lexem_get_type("EOF"), NULL));
	token = tokenize_non_word(input);
	if (token == NULL)
		token = tokenize_word(input);
	return token;
}

t_array tokenize(t_string *input)
{
	add_scope();
	t_array tokens;
	t_token	*token;

	array_init(&tokens);
	while(1)
	{
		token = token_next(input);
		array_push(&tokens, token);
		if (token->type == lexem_get_type("EOF"))
			break;
	}
	return tokens;
	end_scope();
}

static void print(void *token_ptr)
{
	t_array *lexems;
	t_token *token;
	char *id;

	lexems = lexems_get_instance();
	token = token_ptr;
	if (token->type)
		id = ((t_lexem *)(lexems->data[token->type - 1]))->identifier;
	else
	 	id = "WORD";
	printf("[%s, %s]\n", id, token->filename);
	fflush(NULL);
}

int is_quoted(t_string *input);
int main(void)
{
	char *line;
	t_string input;
	t_array tokens;

	string_init(&input);
	/*string_append(&input, "ls -a&&echo 'jj'$arg\"here\"x | cat < hello << LL > -n >> out | (exit && true) > out_exit || (env && export)");*/
	while(1)
	{
		line = readline("minishell> ");
		add_history(line);
		string_set(&input, line);
		if (!is_quoted(&input))
			report_error("minishell: unbalanced quotes!");
		tokens = tokenize(&input);
		array_do(&tokens, print);
	}
}

int is_quoted(t_string *input)
{
  int balance;
  char in_quote;
  char c;

  balance = 0;
  in_quote = '\0';
  while (1)
  {
    c = string_peek_advance(input);
    if (c == '\0')
      break;
    if (in_quote != '\'' && c == '\\')
      string_peek_advance(input);
    else if (in_quote == '\0' && ( c == '"' || c == '\''))
    {
      in_quote = c;
      balance = 1; 
    }
    else if (in_quote == c)
    {
      in_quote = '\0';
      balance = 0;
    }
  }
  string_peek_reset(input);
  return (balance == 0);
}
