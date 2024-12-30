/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrochd <rrochd@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 16:38:30 by rrochd            #+#    #+#             */
/*   Updated: 2024/12/30 22:10:55 by rrochd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "minishell.h"
#include "libft/containers/array/array.h"
#include "tokenizer.h"
#include "ast.h"
#include <readline/history.h>
#include <readline/readline.h>

/*static void	print(void *token_ptr)*/
/*{*/
/*	t_array	*lexems;*/
/*	t_token	*token;*/
/*	char	*id;*/
/*	lexems = lexems_get_instance();*/
/*	token = token_ptr;*/
/*	if (token->type == 0)*/
/*		id = "WORD";*/
/*	else if(token->type == -1)*/
/*		id = "EOF";*/
/*	else*/
/*		id = ((t_lexem *)(lexems->data[token->type - 1]))->identifier;*/
/*	printf("[%s, %s]\n", id, token->filename);*/
/*	fflush(NULL);*/
/*}*/

void print3(void *ptr)
{
	t_node_command *array;

	array = ptr;
	printf("%d\n", array->type);
}

void print2(void *ptr)
{
	t_compound_node *array;

	array = ptr;
	if (array->type == AST_PIPELINE && array->pipeline->size > 0)
		array_do(array->pipeline, print3);
}

void print(void *array_ptr)
{
	t_array *array;

	array = array_ptr;
	printf("%ld\n", array->size);
	array_do(array, print2);
}

int	main(void)
{
	/*char		*line;*/
	t_string	input;
	t_array		*tokens;

	string_init(&input);
	string_set(&input, "ls -a 'xs' && ls | sl | wi && (ls jds) > dwi");
	tokens = tokenize(&input);
	t_array *list = command_list(tokens);
	array_do(list, print);
	/*while (1)*/
	/*{*/
	/*	line = readline("minishell> ");*/
	/*	add_history(line);*/
	/*	string_set(&input, line);*/
	/*	tokens = tokenize(&input);*/
	/*	command_list(tokens);*/
	/*	if (tokens != NULL)*/
	/*		array_do(tokens, print);*/
	/*	free(line);*/
	/*}*/
	/*rl_clear_history();*/
	manager_free_everything();
}
