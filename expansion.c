#include "expansion.h"

char	*get_param_value(char *arg_name)
{
	char *arg_value;
	arg_value = getenv(arg_name);
	if (arg_value == NULL)
		return ("");
	return (arg_value);
}

void	token_peek(t_token *token, char *c, char *m)
{
	*c = string_peek(token->value);
	*m = string_peek(token->mask);
}

void	token_peek_advance(t_token *token)
{
	string_peek_advance(token->value);
	string_peek_advance(token->mask);
}

void	token_peek_reset(t_token *token)
{
	string_peek_reset(token->value);
	string_peek_reset(token->mask);
}

void	token_segment_remove(t_token *token, size_t start, size_t length)
{
	string_segment_remove(token->value, start, length);
	string_segment_remove(token->mask, start, length);		
}

static bool	is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

static bool	is_valid_param_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

void	token_update_mask(t_token *token, size_t start, size_t length, size_t value_length)
{
	char 	*value_mask;

	value_mask = track_malloc((value_length + 1) * sizeof(char));
	ft_memset(value_mask, '1', value_length);
	string_segment_replace(token->mask, start, length, value_mask);
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
	token_peek_reset(token);
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
	token_peek_reset(token);
	return (field_token);
}

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

t_field *field_splitting(t_token *token)
{
	t_field *field;
	char in_quote;
	char c;
	char m;

	field = field_init(token->type);
	token_peek_reset(token);
	while (true)
	{
		token_peek(token, &c, &m);
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

void	array_merge(t_array *dest, t_array *src)
{
	size_t	i;

	i = 0;
	while (i < src->size)
	{
		array_push(dest, src->data[i]);
		i++;
	}
}

void	expansion(t_ast_node *node)
{
	t_field		*field;
	t_array		*argument_list;
	t_array		*redirect_list;
	size_t		i;

	argument_list = track_malloc(sizeof(t_array));
	redirect_list = track_malloc(sizeof(t_array));
	array_init(argument_list);
	array_init(redirect_list);
	array_do(node->children, parameter_expansion);// always results in a single field.
	i = 0;
	while (i < node->children->size)
	{
		//TODO: field might be empty.
		field = field_splitting(node->children->data[i]);
		if (lexem_is_redirection(field->type))
			array_push(redirect_list, field->tokens);
		else
			array_merge(argument_list, field->tokens);
		i++;
	}
	node->children = argument_list;
	node->redirect_list = redirect_list;
}

void token_remove_quotes(void *token_ptr)
{
	t_token *token;
	char 	in_quote;
	char 	c;
	char	m;

	token = token_ptr;
	token_peek_reset(token);
	while (true)
	{
		token_peek(token, &c, &m);
		if (c == '\0')
			break;
		if (in_quote == '\0' && m == '0' && (c == '"' || c == '\''))
		{
			in_quote = c;
			token_segment_remove(token, token->value->peek, 1);
		}
		else if (in_quote == c && m == '0')
		{
			in_quote = '\0';
			token_segment_remove(token, token->value->peek, 1);
		}
		else
			token_peek_advance(token);
	}
}

void	quote_removal(t_ast_node *node)
{
	t_array	*tokens;
	size_t	i;

	array_do(node->children, token_remove_quotes);
	i = 0;
	while (i < node->redirect_list->size)
	{
		tokens = node->redirect_list->data[i];
		array_do(tokens, token_remove_quotes);
		i++;
	}
}
