/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast-redirect.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 16:24:47 by abessa-m          #+#    #+#             */
/*   Updated: 2025/04/12 16:27:05 by abessa-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static t_redirect	*create_redirect(t_token **tokens);
static t_ast_node	*link_redirection_node(
						t_ast_node *cmd_node, t_redirect *redir);

t_ast_node	*parse_redirections(t_token **tokens)
{
	t_ast_node	*cmd_node;
	t_redirect	*redir;

	cmd_node = parse_commands(tokens);
	if (!cmd_node)
		return (NULL);
	while (*tokens && ((*tokens)->type == e_INPUT_REDIRECTION
			|| (*tokens)->type == e_OUTPUT_REDIRECTION
			|| (*tokens)->type == e_APPEND || (*tokens)->type == e_HERE_DOC))
	{
		redir = create_redirect(tokens);
		if (!redir)
		{
			free_ast_node(cmd_node);
			return (NULL);
		}
		cmd_node = link_redirection_node(cmd_node, redir);
		if (!cmd_node)
			return (NULL);
	}
	return (cmd_node);
}

static t_redirect	*create_redirect(t_token **tokens)
{
	t_redirect	*redir;

	redir = malloc(sizeof(t_redirect));
	if (!redir)
		return (NULL);
	redir->redirect_type = (*tokens)->type;
	*tokens = (*tokens)->next;
	if (!*tokens || (*tokens)->type != e_WORD)
	{
		free(redir);
		return (NULL);
	}
	redir->file = strdup((*tokens)->token_string);
	if (!redir->file)
	{
		free(redir);
		return (NULL);
	}
	*tokens = (*tokens)->next;
	return (redir);
}

static t_ast_node	*link_redirection_node(
	t_ast_node *cmd_node, t_redirect *redir)
{
	t_ast_node	*redir_node;

	redir_node = create_ast_node(AST_REDIRECT, redir);
	if (!redir_node)
	{
		free(redir->file);
		free(redir);
		return (NULL);
	}
	redir_node->left = cmd_node;
	return (redir_node);
}
