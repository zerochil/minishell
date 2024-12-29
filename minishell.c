#include "minishell.h"
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

typedef struct s_redirection
{
	char *filename;
	int  (*open_file)(t_token*, void*);
}	t_redirection;

struct s_token
{
	int	type;
	union
	{
		char *word;
		t_redirection *redirection;
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
} t_lexem;


void lexems_init(t_array*);
t_array *token_get_instance();

void lexem_add(char* symbol, char *identifier)
{
	static int id = 1;
	t_array *lexems;
	t_lexem *lexem;

	lexems = token_get_instance();
	lexem = track_malloc(sizeof(t_lexem));
	lexem->type = id;
	lexem->symbol = symbol;
	lexem->identifier = identifier;
	array_push(lexems, lexem);
	id++;
}

// && || >> << > < ( ) |
void lexems_init(t_array *lexems)
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
	lexem_add("|", "PIPE");
	lexem_add("", "EOF");
}

void lexems_destroyer(void *ptr)
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

// next_token
// token.type == lexem_get_type("")

int lexem_match(void *_lexem, void *_identifier)
{
	t_lexem *lexem;
	char *identifier;
	size_t lexem_id_len;

	lexem = _lexem;
	identifier = _identifier;
	lexem_id_len = ft_strlen(lexem->identifier);
	return (ft_strncmp(lexem->identifier, identifier, lexem_id_len) == 0);
}

int lexem_get_type(char *identifier)
{
	t_array *lexems = lexems_get_instance();
	t_lexem *lexem;

	lexem = array_find(lexems, identifier, lexem_match);
	if (lexem == NULL)
		return (0);
	return (lexem->type);
	// find identifier's type using array_find;
	// report_error if identifier isn't found
}

t_token *token_next()
{
	token = token_match
	if (token == NULL)
		token = tokenize_word()
	if (token == NULL)
		token = get_token("EOF");
	return token;
}


// 





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

// ls && (echo && echo) || ls | ls

//parse -> grouping
//grouping -> - '(' simple_list ')' | simple_list | pipeline
//simple_list -> - grouping '&&' grouping
//               - grouping '||' grouping
//pipeline -> command '|' pipeline
//         -> command
//

int main(void)
{
  char *line;
  t_string input;

  string_init(&input);
  while(1)
  {
    line = readline("minishell> ");
    add_history(line);
    string_set(&input, line);
    if (!is_quoted(&input))
       report_error("minishell: unbalanced quotes!");
  }
}
