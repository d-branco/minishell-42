/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 15:07:05 by abessa-m          #+#    #+#             */
/*   Updated: 2025/05/07 10:35:29 by abessa-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_command(t_ast_node *node, t_mnsh *shell);
int	execute_and(t_ast_node *node, t_mnsh *shell);
int	execute_or(t_ast_node *node, t_mnsh *shell);
int	execute_pipe(t_ast_node *node, t_mnsh *shell);
int	execute_redirect(t_ast_node *node, t_mnsh *shell);
int	handle_input_redirection(
		t_redirect *redir, t_ast_node *node, t_mnsh *shell);

int	execute_ast(t_ast_node *node, t_mnsh *shell)
{
	if (!node)
		return (0);
	handle_exit_code(0);
	if (node->type == AST_COMMAND)
		handle_exit_code(execute_command(node, shell));
	else if (node->type == AST_PIPE)
		handle_exit_code(execute_pipe(node, shell));
	else if (node->type == AST_REDIRECT)
		handle_exit_code(execute_redirect(node, shell));
	else if (node->type == AST_AND)
		handle_exit_code(execute_and(node, shell));
	else if (node->type == AST_OR)
		handle_exit_code(execute_or(node, shell));
	else
	{
		ft_putstr_fd("AST EXECUTION ERROR: On the TODO list!\n", 2);
		return (1);
	}
	return (handle_exit_code(-1));
}

////////////////////////////////////////////////////////////////////////////////
int	execute_redirect(t_ast_node *node, t_mnsh *shell)
{
	t_redirect	*redirection;

	redirection = (t_redirect *)node->content;
	if (!redirection)
		return (1);
	if (redirection->redirect_type == e_INPUT_REDIRECTION) //		<
		return (handle_input_redirection(redirection, node, shell));
	else if (redirection->redirect_type == e_OUTPUT_REDIRECTION) //	>
	{
		// OUTPUT REDIRECTION >
	}
	else if (redirection->redirect_type == e_APPEND) //				>>
	{
		// APPEND >>
	}
	else if (redirection->redirect_type == e_HERE_DOC) //			<<
	{
		// HEREDOC <<
	}
	handle_exit_code(execute_ast(node->left, shell));
	return (handle_exit_code(-1));
}

int	handle_input_redirection(t_redirect *redir, t_ast_node *node, t_mnsh *shell)
{
	int	fd;
	int	original_stdin;

	fd = open(redir->file, O_RDONLY);
	if (fd == -1)
		return (perror("minishell: input redirection"), 1);
	original_stdin = dup(STDIN_FILENO);
	if (original_stdin == -1)
		return (perror("minishell: dup"), close(fd), 1);
	if (dup2(fd, STDIN_FILENO) == -1)
		return (perror("minishell: dup2"), close(fd), close(original_stdin), 1);
	close(fd);
	handle_exit_code(execute_ast(node->left, shell));
	dup2(original_stdin, STDIN_FILENO);
	close(original_stdin);
	return (handle_exit_code(-1));
}

////////////////////////////////////////////////////////////////////////////////
int	execute_command(t_ast_node *node, t_mnsh *shell)
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
		{
			printf("--DEBUG-- Hurray! A new child is born!\n");
			print_ast(node, 0);
		}
		execvp(cmd->command, cmd->args);// proibida
		fprintf(stderr, "minishell: %s: command not found\n", cmd->command);
		handle_exit_code(127);
		if (DEBUG)
			printf("--DEBUG-- Did a child had to be killed today?\n");
		free_ast_node(shell->ast_head);
		free_shell(shell);
		exit (handle_exit_code(-1));
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

int	execute_and(t_ast_node *node, t_mnsh *shell)
{
	int	left_status;

	left_status = execute_ast(node->left, shell);
	if (left_status == 0)
		return (execute_ast(node->right, shell));
	return (left_status);
}

int	execute_or(t_ast_node *node, t_mnsh *shell)
{
	int	left_status;

	left_status = execute_ast(node->left, shell);
	if (left_status != 0)
		return (execute_ast(node->right, shell));
	return (left_status);
}

////////////////////////////////////////////////////////////////////////////////

static void	run_left_child(t_ast_node *node, int *pipefd, t_mnsh *shell)
{
	close(pipefd[0]);
	dup2(pipefd[1], STDOUT_FILENO);
	close(pipefd[1]);
	if (DEBUG)
	{

		printf("--DEBUG-- Did the left child had to be killed today?\n");
		print_ast(node->left, 0);
	}
	handle_exit_code(execute_ast(node->left, shell));
	free_ast_node(shell->ast_head);
	free_shell(shell);
	if (DEBUG)
		printf("--DEBUG-- __________________________________________\n");
	exit (handle_exit_code(-1));
}

static void	run_right_child(t_ast_node *node, int *pipefd, t_mnsh *shell)
{
	close(pipefd[1]);
	dup2(pipefd[0], STDIN_FILENO);
	close(pipefd[0]);
	if (DEBUG)
	{

		printf("--DEBUG-- Did the right child had to be killed today?\n");
		print_ast(node->right, 0);
	}
	handle_exit_code(execute_ast(node->right, shell));
	free_ast_node(shell->ast_head);
	free_shell(shell);
	if (DEBUG)
		printf("--DEBUG-- __________________________________________\n");
	exit (handle_exit_code(-1));
}

static int	wait_for_child(pid_t pid)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		return (handle_exit_code(WEXITSTATUS(status)));
	return (handle_exit_code(1));
}

int	execute_pipe(t_ast_node *node, t_mnsh *shell)
{
	int		pipefd[2];
	pid_t	left_pid;
	pid_t	right_pid;
	int		right_status;

	if (pipe(pipefd) == -1)
	{
		perror("minishell: pipe");
		return (1);
	}
	left_pid = fork();
	if (left_pid == -1)
		return (perror("minishell: fork"), 1);
	if (left_pid == 0)
		run_left_child(node, pipefd, shell);
	right_pid = fork();
	if (right_pid == -1)
		return (perror("minishell: fork"), 1);
	if (right_pid == 0)
		run_right_child(node, pipefd, shell);
	close(pipefd[0]);
	close(pipefd[1]);
	wait_for_child(left_pid);
	right_status = wait_for_child(right_pid);
	return (handle_exit_code(right_status));
}

////////////////////////////////////////////////////////////////////////////////
