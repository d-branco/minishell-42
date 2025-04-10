/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 18:27:31 by abessa-m          #+#    #+#             */
/*   Updated: 2025/04/10 09:25:01 by abessa-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_ast_node *build_ast(t_token **tokens);
//t_ast_node *parse_logical_ops(t_token **tokens);
//t_ast_node *parse_pipe(t_token **tokens);
//t_ast_node *parse_redirections(t_token **tokens);
t_ast_node *parse_commands(t_token **tokens);

t_ast_node	*build_ast(t_token **tokens)
{
	if (!tokens || !*tokens)
		return (NULL);
	return (parse_logical_ops(tokens));
}

t_ast_node	*parse_logical_ops(t_token **tokens)
