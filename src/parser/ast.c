/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 18:27:31 by abessa-m          #+#    #+#             */
/*   Updated: 2025/04/10 10:00:51 by abessa-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_ast_node	*build_ast(t_token **tokens);
t_ast_node	*parse_logical_ops(t_token **tokens);
t_ast_node	*parse_pipe(t_token **tokens);
//t_ast_node	*parse_redirections(t_token **tokens);
//t_ast_node	*parse_commands(t_token **tokens);

t_ast_node	*build_ast(t_token **tokens)
{
	if (!tokens || !*tokens)
		return (NULL);
	return (parse_logical_ops(tokens));
}

t_ast_node	*parse_logical_ops(t_token **tokens)
{
	t_ast_node	*left;
	t_ast_node	*right;
	t_ast_node	*op_node;
	t_ast_type	op_type;

	left = parse_pipe(tokens);// First parse the pipe expressions on the left
	if (!left)
		return (NULL);
	while (*tokens && ((*tokens)->type == e_AND || (*tokens)->type == e_OR))
	{
		op_type = AST_AND;
		if ((*tokens)->type == e_OR)
			op_type = AST_OR;
		*tokens = (*tokens)->next;
		right = parse_pipe(tokens);
		if (!right)
			return (free_ast_node(left), NULL);
		op_node = create_ast_node(op_type, NULL);
		if (!op_node)
			return (free_ast_node(left), free_ast_node(right), NULL);
		op_node->left = left;// Set up the node
		op_node->right = right;
		left = op_node;
	}
	return (left);
}

t_ast_node	*parse_pipe(t_token **tokens)
{
	t_ast_node	*left;
	t_ast_node	*right;
	t_ast_node	*pipe_node;

	left = parse_redirections(tokens);
	if (!left)
		return (NULL);
	while (*tokens && (*tokens)->type == e_PIPE)
	{
		*tokens = (*tokens)->next;// Move past the pipe token
		right = parse_redirections(tokens);
		if (!right)
			return (free_ast_node(left), NULL);
		pipe_node = create_ast_node(AST_PIPE, NULL);
		if (!pipe_node)
			return (free_ast_node(left), free_ast_node(right), NULL);
		pipe_node->left = left;
		pipe_node->right = right;
		left = pipe_node;
	}
	return (left);
}
