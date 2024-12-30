
#include "../tokenizer.h"

static void	print_token(void *token_ptr)
{
	t_array	*lexems;
	t_token	*token;
	char	*id;

	lexems = lexems_get_instance();
	token = token_ptr;
	if (token->type == 0)
		id = "WORD";
	else if (token->type == -1)
		id = "EOF";
	else
		id = ((t_lexem *)(lexems->data[token->type - 1]))->identifier;
	printf("[%s, %s] ", id, token->filename);
}

int main(int ac, char **av)
{
	int	fd;
	int i = 1;
	char *line;
	t_string input;
	t_array *tokens;

	if (ac != 2)
		return 1;
	string_init(&input);
	fd = open(av[ac - 1], O_RDONLY);
	while (1)
	{
		line = get_next_line(fd);
		if (line == NULL)
			break;
		string_set(&input, line);
		input.data[input.size - 1] = '\0';
		tokens = tokenize(&input);
		printf("%3d. ", i);
		array_do(tokens, print_token);
		printf("\n");
		i++;
	}
	close(fd);
	return 0;
}

