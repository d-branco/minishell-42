/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 13:46:47 by abessa-m          #+#    #+#             */
/*   Updated: 2025/06/26 01:23:06 by alde-alm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	parser(char *input, t_mnsh *shell)
{
	t_token		*list_o_tokens;
	t_token		*list_o_tokens_origin;
	t_ast_node	*ast;

	if (validate_syntax(input) == SYNTAX_ERROR)
	{
		ft_dprintf(2, "Minishell: Syntax error: %s\n", input);
		return (handle_exit_code(2));
	}
	list_o_tokens = NULL;
	parse_input_into_token_list(&list_o_tokens, input);
	tkn_lst_printer(list_o_tokens);

	if (check_token_syntax_errors(list_o_tokens) == SYNTAX_ERROR)
	{
		tkn_lstclear(&list_o_tokens);
		return (handle_exit_code(2));
	}

	list_o_tokens_origin = list_o_tokens;
	ast = build_ast(&list_o_tokens);
	tkn_lstclear(&list_o_tokens_origin);

	if (DEBUG)
		print_ast(ast, 0);

	shell->ast_head = ast;
	execute_ast(shell->ast_head, shell);
	free_ast_node(shell->ast_head);

	return (handle_exit_code(-1));
}

int	is_operator(t_token *t)
{
	return (t->type == e_PIPE || t->type == e_AND || t->type == e_OR);
}

int	is_redirection(t_token *t)
{
	return (t->type == e_OUTPUT_REDIRECTION || t->type == e_APPEND
		|| t->type == e_INPUT_REDIRECTION || t->type == e_HERE_DOC);
}

int	check_token_syntax_errors(t_token *lst)
{
	t_token	*curr;

	curr = lst;
	if (!curr)
		return (0);
	if (is_operator(curr) || is_redirection(curr))
		return (print_unexpected_token(curr));
	while (curr && curr->next)
	{
		if (is_operator(curr) && is_operator(curr->next))
			return (print_unexpected_token(curr->next));
		if (is_redirection(curr) && curr->next->type != e_WORD)
			return (print_unexpected_token(curr->next));
		curr = curr->next;
	}
	if (is_operator(curr) || is_redirection(curr))
		return (print_unexpected_token(NULL));
	return (0);
}

int	print_unexpected_token(t_token *tkn)
{
	if (!tkn)
		ft_putstr_fd("minishell: syntax error near unexpected token `newline'\n", 2);
	else
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
		ft_putstr_fd(tkn->token_string, 2);
		ft_putstr_fd("'\n", 2);
	}
	handle_exit_code(SYNTAX_ERROR);
	return (SYNTAX_ERROR);
}
