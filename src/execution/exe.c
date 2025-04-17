/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 15:07:05 by abessa-m          #+#    #+#             */
/*   Updated: 2025/04/17 16:23:30 by abessa-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_command(t_ast_node *node);
int	execute_and(t_ast_node *node);
int	execute_or(t_ast_node *node);

int	execute_ast(t_ast_node *node)
{
	if (!node)
		return (0);
	handle_exit_code(0);
	if (node->type == AST_COMMAND)
		handle_exit_code(execute_command(node));
	//else if (node->type == AST_PIPE)
	//	handle_exit_code(execute_pipe(node));
	//else if (node->type == AST_REDIRECT)
	//	handle_exit_code(execute_redirect(node));
	else if (node->type == AST_AND)
		handle_exit_code(execute_and(node));
	else if (node->type == AST_OR)
		handle_exit_code(execute_or(node));
	else
	{
		ft_putstr_fd("AST EXECUTION ERROR: On the TODO list!\n", 2);
		return (1);
	}
	return (handle_exit_code(-1));
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
		if (DEBUG)
			printf("--DEBUG--Hurray! A new child is born!\n");
		execvp(cmd->command, cmd->args);
		fprintf(stderr, "minishell: %s: command not found\n", cmd->command);
		handle_exit_code(127);
		if (DEBUG)
			printf("--DEBUG-- Did they had to die today?\n");
		free_ast_node(node);
		exit (127);
	}
	else if (pid < 0)
	{
		perror("minishell: fork");
		return (1);
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		return (handle_exit_code(WEXITSTATUS(status)));
	return (handle_exit_code(-1));
}

int	execute_and(t_ast_node *node)
{
	int	left_status;

	left_status = execute_ast(node->left);
	if (left_status == 0)
		return (execute_ast(node->right));
	return (left_status);
}

int	execute_or(t_ast_node *node)
{
	int	left_status;

	left_status = execute_ast(node->left);
	if (left_status != 0)
		return (execute_ast(node->right));
	return (left_status);
}
