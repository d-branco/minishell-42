/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 13:46:47 by abessa-m          #+#    #+#             */
/*   Updated: 2025/04/15 16:15:37 by abessa-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	parser(char *input)
{
	t_token		*list_o_tokens;
	t_token		*list_o_tokens_origin;
	t_ast_node	*ast;

	list_o_tokens = NULL;
	parse_input_into_token_list(&list_o_tokens, input);
	tkn_lst_printer(list_o_tokens);
	list_o_tokens_origin = list_o_tokens;
	ast = build_ast(&list_o_tokens);
	tkn_lstclear(&list_o_tokens_origin);
	if (DEBUG)
		print_ast(ast, 0);
	execute_ast(ast);
	free_ast_node(ast);
	return (handle_exit_code(-1));
}

//	Parsing order:
//1. Logic operators	||, &&
//2. Pipes				|
//3. Redirections		<, <<, >, >>
//4. Commands			et cetera
