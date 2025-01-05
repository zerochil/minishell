#include "expansion.h"

char ***get_env_instance(void)
{
	static char **env;
	return &env;
}

char	*get_param_value(char *arg_name)
{
	size_t	i;
	size_t	arg_name_len;
	char **env;

	env = *get_env_instance();
	arg_name_len = ft_strlen(arg_name);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], arg_name, arg_name_len) == 0 && env[i][arg_name_len] == '=')
			return env[i] + arg_name_len + 1;
		i++;
	}
	return ("");
}

void	token_update_mask(t_token *token, size_t start, size_t length, size_t value_length)
{
	t_string new_mask;
	char 	*value_mask;

	string_init(&new_mask);
	string_set(&new_mask, token->mask);
	value_mask = track_malloc((value_length + 1) * sizeof(char));
	ft_memset(value_mask, '1', value_length);
	string_segment_replace(&new_mask, start, length, value_mask);
	token->mask = new_mask.data;
}

bool	is_valid_param_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}


void	token_replace_param(t_token *token, t_string *string)
{
	char *param_name;
	char *param_value;
	size_t	value_length;
	size_t	start;

	start = string->peek;
	string_peek_advance(string);
	while (is_valid_param_char(string_peek(string)))
		string_peek_advance(string);
	if (start + 1 == string->peek)
		return ;
	param_name = string_segment_extract(string, start, string->peek - start);
	param_value = get_param_value(param_name + 1);//skip the '$' 
	value_length = ft_strlen(param_value);
	string_segment_replace(string, start, string->peek - start, param_value);
	token_update_mask(token, start, string->peek - start, value_length);
	string->peek = start + value_length;
}

//TODO: what if the variable name starts with a digit?
void	parameter_expansion(void *token_ptr)
{
	t_token		*token;
	t_string	string;
	char		in_quote;
	char		c;

	in_quote = '\0';
	token = token_ptr;
	string_init(&string);
	string_set(&string, token->word);
	while (true)
	{
		c = string_peek(&string);
		if (c == '\0')
			break ;
		if (in_quote == '\0' && (c == '"' || c == '\''))
			in_quote = c;
		else if (in_quote == c)
			in_quote = '\0';
		if (c == '$' && in_quote != '\'')
			token_replace_param(token, &string);
		else
			string_peek_advance(&string);
	}
	token->word = string.data;
}

static bool	is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

t_token	*token_extract(t_token *token, t_string *string, t_string *mask)
{
	t_token *field;

	if (string->peek == 0)
		return (NULL);

	field = track_malloc(sizeof(t_token));
	field->type = token->type;
	field->word = string_segment_slice(string, 0, string->peek);
	field->mask = string_segment_slice(mask, 0, mask->peek);
	string_peek_reset(string);
	string_peek_reset(mask);
	return (field);
}

typedef struct s_field
{
	int		type;
	t_array *tokens;
}	t_field;

t_field *field_init(int type)
{
	t_field	*field;

	field = track_malloc(sizeof(t_field));
	field->type = type;
	field->tokens = track_malloc(sizeof(t_array));
	array_init(field->tokens);
	return (field);
}

void	field_push_token(t_field *field, t_token *token, t_string *string, t_string *mask)
{
	t_token *field_token;

	field_token = token_extract(token, string, mask);
	if (field_token)
		array_push(field->tokens, field_token);
	while (is_whitespace(string_peek(string)) && string_peek(mask) == '1')
	{
		string_shift(string);
		string_shift(mask);
	}
}

t_field *field_splitting(t_token *token)
{
	t_field *field;
	t_string string;
	t_string mask;
	char in_quote;
	char c;
	char m;

	field = field_init(token->type);
	string_init(&string);
	string_set(&string, token->word);
	string_init(&mask);
	string_set(&mask, token->mask);
	while (true)
	{
		c = string_peek(&string);
		m = string_peek(&mask);
		if (c == '\0')
			break;
		if (in_quote == '\0' && m == '0' && (c == '"' || c == '\''))
			in_quote = c;
		else if (in_quote == c && m == '0')
			in_quote = '\0';
		if (is_whitespace(c) && m == '1' && in_quote == '\0')
			field_push_token(field, token, &string, &mask);
		else
		{
			string_peek_advance(&string);
			string_peek_advance(&mask);
		}
	}
	if (string.size > 0)
	{
		token->word = string.data;
		token->mask = mask.data;
		array_push(field->tokens, token);
	}
	return (field);
}

void	split_fields(void *token_ptr)
{
	t_token *token;
	t_field *field;

	token = token_ptr;
	field = field_splitting(token);
	if (lexem_is_redirection(field->type) && field->tokens->size > 1)
		printf("Ambiguos redirection\n");
	else
		array_do(field->tokens, print_token);
}

