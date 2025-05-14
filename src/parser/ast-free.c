/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast-free.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 16:28:28 by abessa-m          #+#    #+#             */
/*   Updated: 2025/05/14 10:39:21 by abessa-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_ast_content(t_ast_node *node);
static void	free_command(t_command *cmd);

void	free_arg_list(t_list *arg_list)
{
	t_list	*tmp;

	while (arg_list)
	{
		tmp = arg_list;
		arg_list = arg_list->next;
		free(tmp->content);
		free(tmp);
	}
}

void	free_arg_list_structure(t_list *arg_list)
{
	t_list	*tmp;

	while (arg_list)
	{
		tmp = arg_list;
		arg_list = arg_list->next;
		free(tmp);
	}
}

void	free_ast_node(t_ast_node *node)
{
	if (!node)
		return ;
	free_ast_node(node->left);
	free_ast_node(node->right);
	free_ast_content(node);
	free(node);
}

static void	free_ast_content(t_ast_node *node)
{
	t_redirect	*redir;

	if (!node || !node->content)
		return ;
	if (node->type == AST_COMMAND)
		free_command((t_command *)node->content);
	else if (node->type == AST_REDIRECT)
	{
		redir = node->content;
		if (!redir)
			return ;
		free(redir->file);
		free(redir);
	}
	else
		free(node->content);
}

static void	free_command(t_command *cmd)
{
	int	i;

	i = 0;
	if (!cmd)
		return ;
	while (cmd->args[i])
		free(cmd->args[i++]);
	free(cmd->args);
	free(cmd);
}
