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
	char 	*value_mask;

	value_mask = track_malloc((value_length + 1) * sizeof(char));
	ft_memset(value_mask, '1', value_length);
	string_segment_replace(token->mask, start, length, value_mask);
}

bool	is_valid_param_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}


void	token_replace_param(t_token *token)
{
	char *param_name;
	char *param_value;
	size_t	value_length;
	size_t	start;

	start = token->value->peek;
	string_peek_advance(token->value);
	while (is_valid_param_char(string_peek(token->value)))
		string_peek_advance(token->value);
	if (start + 1 == token->value->peek)
		return ;
	param_name = string_segment_extract(token->value, start, token->value->peek - start);
	param_value = get_param_value(param_name + 1);//skip the '$' 
	value_length = ft_strlen(param_value);
	string_segment_replace(token->value, start, token->value->peek - start, param_value);
	token_update_mask(token, start, token->value->peek - start, value_length);
	token->value->peek = start + value_length;
}

//TODO: what if the variable name starts with a digit?
void	parameter_expansion(void *token_ptr)
{
	t_token		*token;
	char		in_quote;
	char		c;

	in_quote = '\0';
	token = token_ptr;
	while (true)
	{
		c = string_peek(token->value);
		if (c == '\0')
			break ;
		if (in_quote == '\0' && (c == '"' || c == '\''))
			in_quote = c;
		else if (in_quote == c)
			in_quote = '\0';
		if (c == '$' && in_quote != '\'')
			token_replace_param(token);
		else
			string_peek_advance(token->value);
	}
	string_peek_reset(token->value);
}


static bool	is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

t_token	*token_slice(t_token *token)
{
	t_token *field_token;
	char	*value;
	char	*mask;

	if (token->value->peek == 0)
		return (NULL);
	field_token = track_malloc(sizeof(t_token));
	field_token->type = token->type;
	value = string_segment_slice(token->value, 0, token->value->peek);
	mask = string_segment_slice(token->mask, 0, token->mask->peek);
	field_token->value = track_malloc(sizeof(t_string));
	field_token->mask = track_malloc(sizeof(t_string));
	string_init(field_token->value);
	string_set(field_token->value, value);
	string_init(field_token->mask);
	string_set(field_token->mask, mask);
	string_peek_reset(token->value);
	string_peek_reset(token->mask);
	return (field_token);
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

void	field_push_token(t_field *field, t_token *token)
{
	t_token *field_token;

	field_token = token_slice(token);
	if (field_token)
		array_push(field->tokens, field_token);
	while (is_whitespace(string_peek(token->value)) && string_peek(token->mask) == '1')
	{
		string_shift(token->value);
		string_shift(token->mask);
	}
}

void	token_peek_advance(t_token *token)
{
	string_peek_advance(token->value);
	string_peek_advance(token->mask);
}

t_field *field_splitting(t_token *token)
{
	t_field *field;
	char in_quote;
	char c;
	char m;

	field = field_init(token->type);
	while (true)
	{
		c = string_peek(token->value);
		m = string_peek(token->mask);
		if (c == '\0')
			break;
		if (in_quote == '\0' && m == '0' && (c == '"' || c == '\''))
			in_quote = c;
		else if (in_quote == c && m == '0')
			in_quote = '\0';
		if (is_whitespace(c) && m == '1' && in_quote == '\0')
			field_push_token(field, token);
		else
			token_peek_advance(token);
	}
	if (token->value->size > 0)
		array_push(field->tokens, token);
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

