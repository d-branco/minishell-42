/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast-parse-cmd2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 17:14:12 by abessa-m          #+#    #+#             */
/*   Updated: 2025/05/14 10:39:21 by abessa-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_ast_node	*handle_tokens_inside_parenthesis(t_token **tokens)
{
	t_ast_node	*subshell;

	*tokens = (*tokens)->next;
	subshell = parse_logical_ops(tokens);
	if (!subshell)
		return (NULL);
	if (!*tokens || (*tokens)->type != e_PARENTHESIS_CLOSE)
	{
		free_ast_node(subshell);
		return (NULL);
	}
	*tokens = (*tokens)->next;
	return (subshell);
}

int	is_valid_token_for_argument(t_token *token)
{
	return (token
		&& (token->type == e_WORD
			|| token->type == e_SINGLE_QUOTE
			|| token->type == e_DOUBLE_QUOTE));
}
