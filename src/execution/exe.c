/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 15:07:05 by abessa-m          #+#    #+#             */
/*   Updated: 2025/04/15 17:02:50 by abessa-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_ast(t_ast_node *node)
{
	t_command	*cmd;
	int			i;

	if (!node)
		return (0);
	handle_exit_code(0);
	if (node->type == AST_COMMAND)
	{
		cmd = (t_command *)node->content;
		i = 0;
		printf("Executa commando: %s", cmd->args[i]);
		i++;
		while (cmd->args[i])
		{
			printf(" %s", cmd->args[i]);
			i++;
		}
		printf("\n");
	}
	else
	{
		execute_ast(node->left);
		execute_ast(node->right);
	}
	return (0);
}
