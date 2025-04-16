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

int	execute_command(t_ast_node *node);

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
		execute_command(node);
	}
	else
	{
		execute_ast(node->left);
		execute_ast(node->right);
	}
	return (0);
}

int	execute_command(t_ast_node *node)
{
	t_command	*cmd;
	pid_t		pid;
	int			status;

	cmd = (t_command *)node->content;
	if (!cmd || !cmd->command)
		return (1);
	pid = fork();
	if (pid == 0)
	{
		execvp(cmd->command, cmd->args);
		fprintf(stderr, "minishell: %s: command not found\n", cmd->command);
		handle_exit_code(127);
	}
	else if (pid < 0)
	{
		perror("minishell: fork");
		return (1);
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}
