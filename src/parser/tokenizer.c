/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 15:22:15 by abessa-m          #+#    #+#             */
/*   Updated: 2025/04/03 15:07:23 by abessa-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

/*
typedef enum e_token_type
{
	TOKEN_SINGLE_QUOTE,
	TOKEN_DOUBLE_QUOTE,
	TOKEN_PARENTHESIS_OPEN,
	TOKEN_PARENTHESIS_CLOSE,
	TOKEN_PIPE,
	TOKEN_AND,
	TOKEN_OR,
	TOKEN_OUTPUT_REDIRECTION,
	TOKEN_APPEND,
	TOKEN_INPUT_REDIRECTION,
	TOKEN_HERE_DOC,
	TOKEN_CMD_ARGS
}	t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*token_string;
	int				length;
}	t_token;
*/

/*Second try:
void	print_token_list_info(void *content)
{
	t_token	*data;
	const char	*type_strings[] = {
		"single_quote", "double_quote",
		"open_parenthesis", "close_parenthesis",
		"pipe", "logical_and", "logical_or",
		"output_redirection", "append", "input_redirection", "here_doc",
		"argument"};

	data = (t_token *)content;
	printf("Token type: %s, Token string: %s\n",
		type_strings[data->type], data->token_string);
}

t_token	*create_token(t_token_type type, const char *token_string)
{
	t_token	*data;

	data = malloc(sizeof(t_token));
	if (!data)
		return (NULL);
	data->type = type;
	data->token_string = strdup(token_string);
	if (DEBUG)
		print_token_list_info(data);
	return (data);
}

void	delete_token(void *content)
{
	t_token	*data;

	data = (t_token *)content;
	free(data->token_string);
	free(data);
}
*/
