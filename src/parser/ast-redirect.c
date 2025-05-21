/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast-redirect.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 16:24:47 by abessa-m          #+#    #+#             */
/*   Updated: 2025/05/21 10:29:50 by abessa-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_redirect	*create_redirect(t_token **tokens);
static t_ast_node	*link_redirection_node(
						t_ast_node *cmd_node, t_redirect *redir);
static t_command	*create_empty_command(void);

t_ast_node	*parse_redirections(t_token **tokens)
{
	t_ast_node	*cmd_node;
	t_redirect	*redir;
	int			is_at_start;
	t_ast_node	*real_cmd;
	t_ast_node	*empty_cmd_node;

	is_at_start = FALSE;
	if (*tokens && ((*tokens)->type == e_INPUT_REDIRECTION
			|| (*tokens)->type == e_OUTPUT_REDIRECTION
			|| (*tokens)->type == e_APPEND || (*tokens)->type == e_HERE_DOC))
		is_at_start = TRUE;
	if (is_at_start)
		cmd_node = create_ast_node(AST_COMMAND, create_empty_command());
	else
		cmd_node = parse_commands(tokens);
	if (!cmd_node)
		return (NULL);
	while (*tokens && ((*tokens)->type == e_INPUT_REDIRECTION
			|| (*tokens)->type == e_OUTPUT_REDIRECTION
			|| (*tokens)->type == e_APPEND || (*tokens)->type == e_HERE_DOC))
	{
		redir = create_redirect(tokens);
		if (!redir)
			return (free_ast_node(cmd_node), NULL);
		cmd_node = link_redirection_node(cmd_node, redir);
		if (!cmd_node)
			return (NULL);
	}
	if (is_at_start && *tokens && is_valid_token_for_argument(*tokens))
	{
		real_cmd = parse_commands(tokens);
		if (real_cmd)
		{
			empty_cmd_node = cmd_node->left;
			cmd_node->left = real_cmd;
			if (empty_cmd_node)
			{
				empty_cmd_node->left = NULL;
				empty_cmd_node->right = NULL;
				free_ast_node(empty_cmd_node);
			}
		}
	}
	return (cmd_node);
}

static t_command	*create_empty_command(void)
{
	t_command	*cmd;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->args = malloc(sizeof(char *) * 1);
	if (!cmd->args)
		return (free(cmd), NULL);
	cmd->args[0] = NULL;
	cmd->command = NULL;
	cmd->argc = 0;
	return (cmd);
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
