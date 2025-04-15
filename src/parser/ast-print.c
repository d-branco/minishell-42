/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast-print.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 16:20:30 by abessa-m          #+#    #+#             */
/*   Updated: 2025/04/12 16:27:32 by abessa-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// For debugging and fun :D

static void	print_indentation(int depth);
static void	print_command(t_command *cmd, int depth);
static void	print_node_type(t_ast_node *node, int depth);

void	print_ast(t_ast_node *node, int depth)
{
	if (!node)
		return ;
	printf("--DEBUG-- Abstract Syntax Tree  ");
	print_indentation(depth);
	print_node_type(node, depth);
	print_ast(node->left, depth + 1);
	print_ast(node->right, depth + 1);
}

static void	print_indentation(int depth)
{
	int	i;

	i = 0;
	while (i < depth)
	{
		printf("|  ");
		i++;
	}
}

static void	print_node_type(t_ast_node *node, int depth)
{
	t_redirect	*redir;

	if (node->type == AST_COMMAND)
		print_command((t_command *)node->content, depth);
	else if (node->type == AST_PIPE)
		printf("PIPE\n");
	else if (node->type == AST_REDIRECT)
	{
		redir = (t_redirect *)node->content;
		printf("REDIRECT(%d): %s\n", redir->redirect_type, redir->file);
	}
	else if (node->type == AST_AND)
		printf("AND\n");
	else if (node->type == AST_OR)
		printf("OR\n");
}

static void	print_command(t_command *cmd, int depth)
{
	int	i;

	i = 0;
	printf("COMMAND: %s\n", cmd->command);
	while (cmd->args[i])
	{
		printf("--DEBUG-- Abstract Syntax Tree  ");
		print_indentation(depth + 1);
		printf("ARG[%d]: %s\n", i, cmd->args[i]);
		i++;
	}
}
