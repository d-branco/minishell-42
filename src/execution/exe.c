/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 15:07:05 by abessa-m          #+#    #+#             */
/*   Updated: 2025/04/15 16:30:54 by abessa-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_ast(t_ast_node *node)
{
	t_command	*cmd;

	if (!node)
		return (0);
	handle_exit_code(0);
	if (node->type == AST_COMMAND)
	{
		cmd = (t_command *)node->content;
		printf("Executa commando: %s\n", cmd->args[0]);
	}
	else
	{
		execute_ast(node->left);
	}
	return (0);
}
