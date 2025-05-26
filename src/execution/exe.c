/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 15:07:05 by abessa-m          #+#    #+#             */
/*   Updated: 2025/05/19 17:05:19 by abessa-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int			execute_command(t_ast_node *node, t_mnsh *shell);
int			execute_and(t_ast_node *node, t_mnsh *shell);
int			execute_or(t_ast_node *node, t_mnsh *shell);
int			execute_pipe(t_ast_node *node, t_mnsh *shell);
int			execute_redirect(t_ast_node *node, t_mnsh *shell);
int			handle_input_redirection(
				t_redirect *redir, t_ast_node *node, t_mnsh *shell);
int			handle_output_redirection(
				t_ast_node *node, t_mnsh *shell, const char *file);
static int	open_output_file(const char *file);
static int	backup_fd(int fd_to_backup);
static int	redirect_fd(int new_fd, int old_fd);
static int	restore_fd(int backup_fd, int original_fd);
void		fd_bug(char *location, int fd, char *action);
static void	execute_command_child(t_command *cmd, t_mnsh *shell);
static char	*resolve_command_path(char *cmd, char **envp);
int			handle_append_redirection(
				t_ast_node *node, t_mnsh *shell, char *file);
int			open_append_file(const char *file);

void	fd_bug(char *location, int fd, char *action)
{
	if (DEBUG)
		printf("--DEBUG-- FD %25s: fd %d - %s\n", location, fd, action);
}

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
		return (handle_output_redirection(node, shell, redirection->file));
	else if (redirection->redirect_type == e_APPEND) //				>>
	{
		return (handle_append_redirection(node, shell, redirection->file));/////
	}
	else if (redirection->redirect_type == e_HERE_DOC) //			<<
	{
		// HEREDOC <<
	}
	handle_exit_code(execute_ast(node->left, shell));
	return (handle_exit_code(-1));
}

////////////////////////////////////////////////////////////////////////////////
int	handle_append_redirection(t_ast_node *node, t_mnsh *shell, char *file)
{
	int	fd;
	int	stdout_temp;
	int	result;

	fd = open_append_file(file);
	if (fd == -1)
		return (1);
	stdout_temp = backup_fd(STDOUT_FILENO);
	if (stdout_temp == -1)
		return (close(fd), 1);
	if (!redirect_fd(fd, STDOUT_FILENO))
		return (close(fd), close(stdout_temp), 1);
	fd_bug("handle_append_redirection", fd, "closing after redirection");
	close(fd);
	result = execute_ast(node->left, shell);
	if (!restore_fd(stdout_temp, STDOUT_FILENO))
		return (close(stdout_temp), 1);
	fd_bug("handle_append_redirection", stdout_temp, "closing after restore");
	close(stdout_temp);
	return (result);
}

int	open_append_file(const char *file)
{
	int	fd;

	fd = open(file, O_CREAT | O_WRONLY | O_APPEND, 0644);
	if (fd == -1)
		return (perror("minishell"), -1);
	fd_bug("open_append_file", fd, "opened");
	return (fd);
}

////////////////////////////////////////////////////////////////////////////////
int	handle_output_redirection(t_ast_node *node, t_mnsh *shell, const char *file)
{
	int	fd;
	int	stdout_temp;
	int	result;

	fd = open_output_file(file);
	if (fd == -1)
		return (1);
	stdout_temp = backup_fd(STDOUT_FILENO);
	if (stdout_temp == -1)
		return (close(fd), 1);
	if (!redirect_fd(fd, STDOUT_FILENO))
		return (close(fd), close(stdout_temp), 1);
	fd_bug("handle_output_redirection", fd, "closing after redirection");
	close(fd);
	result = execute_ast(node->left, shell);
	if (!restore_fd(stdout_temp, STDOUT_FILENO))
		return (close(stdout_temp), 1);
	fd_bug("handle_output_redirection", stdout_temp, "closing after restore");
	close(stdout_temp);
	return (result);
}

static int	open_output_file(const char *file)
{
	int	fd;

	fd = open(file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == -1)
		return (perror("minishell"), -1);
	fd_bug("open_output_file", fd, "opened");
	return (fd);
}

static int	backup_fd(int fd_to_backup)
{
	int	backup;

	backup = dup(fd_to_backup);
	if (backup == -1)
		return (perror("minishell"), -1);
	fd_bug("backup_fd", backup, "created as backup");
	return (backup);
}

static int	redirect_fd(int new_fd, int old_fd)
{
	fd_bug("redirect_fd", old_fd, "before redirection");
	if (dup2(new_fd, old_fd) == -1)
		return (perror("minishell"), 0);
	fd_bug("redirect_fd", old_fd, "after redirection");
	return (1);
}

static int	restore_fd(int backup_fd, int original_fd)
{
	fd_bug("restore_fd", original_fd, "before restoration");
	if (dup2(backup_fd, original_fd) == -1)
		return (perror("minishell"), 0);
	fd_bug("restore_fd", original_fd, "after restoration");
	return (1);
}

////////////////////////////////////////////////////////////////////////////////
int	handle_input_redirection(t_redirect *redir, t_ast_node *node, t_mnsh *shell)
{
	int	fd;
	int	original_stdin;

	fd = open(redir->file, O_RDONLY);
	if (fd == -1)
		return (perror("minishell: input redirection"), 1);
	fd_bug("handle_input_redirection", fd, "opened");
	original_stdin = dup(STDIN_FILENO);
	if (original_stdin == -1)
		return (perror("minishell: dup"), close(fd), 1);
	fd_bug("handle_input_redirection", original_stdin, "created as backup");
	if (dup2(fd, STDIN_FILENO) == -1)
		return (perror("minishell: dup2"), close(fd), close(original_stdin), 1);
	fd_bug("handle_input_redirection", STDIN_FILENO, "after redirection");
	fd_bug("handle_input_redirection", fd, "closing after redirection");
	close(fd);
	handle_exit_code(execute_ast(node->left, shell));
	dup2(original_stdin, STDIN_FILENO);
	if (dup2(original_stdin, STDIN_FILENO) == -1)
		return (perror("minishell: dup2 restore"), close(original_stdin), 1);
	fd_bug("handle_input_redirection", STDIN_FILENO, "after restoration");
	fd_bug("handle_input_redirection", original_stdin, "closing after restore");
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
	if (is_builtin(cmd))
	{
		if (DEBUG)
			ft_printf("--DEBUG-- built-in		%s\n", cmd->command);
		return (execute_builtin(cmd, shell));
	}
	pid = fork();
	if ((DEBUG) && (pid == 0))
		print_ast(node, 0);
	if (pid == 0)
		execute_command_child(cmd, shell);
	else if (pid < 0)
		return (perror("minishell: fork"), 1);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		return (handle_exit_code(WEXITSTATUS(status)));
	return (handle_exit_code(-1));
}

static void	execute_command_child(t_command *cmd, t_mnsh *shell)
{
	char	*full_path;

	full_path = resolve_command_path(cmd->command, shell->envp);
	if (!full_path)
	{
		fprintf(stderr, "minishell: %s: command not found\n", cmd->command);
		handle_exit_code(127);
		free_ast_node(shell->ast_head);
		free_shell(shell);
		//exit (handle_exit_code(-1));
	}
	execve(full_path, cmd->args, shell->envp);
	//perror("minishell");
	free(full_path);
	free_ast_node(shell->ast_head);
	free_shell(shell);
	exit (handle_exit_code(-1));
}

static char	*build_full_path(char *path, char *cmd)
{
	char	*full_path;

	full_path = malloc(strlen(path) + strlen(cmd) + 2);
	if (!full_path)
		return (NULL);
	ft_strlcpy(full_path, path, strlen(path) + 1);
	ft_strlcat(full_path, "/", strlen(path) + 2);
	ft_strlcat(full_path, cmd, strlen(path) + strlen(cmd) + 2);
	return (full_path);
}

static void	free_array(char **array)
{
	int	i;

	i = 0;
	if (!array)
		return ;
	while (array[i])
		free(array[i++]);
	free(array);
}

static char	*resolve_command_path(char *cmd, char **envp)
{
	char	**paths;
	char	*path_var;
	char	*full_path;
	int		i;

	if (ft_strchr(cmd, '/')) // comando com caminho direto
		return (ft_strdup(cmd));
	path_var = ft_getenv(envp, "PATH=");
	if (!path_var)
		return (NULL);
	paths = ft_split(path_var, ':');
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		full_path = build_full_path(paths[i], cmd);
		if (access(full_path, X_OK) == 0)
			return (free_array(paths), full_path);
		free(full_path);
		i++;
	}
	free_array(paths);
	return (NULL);
}

/*
static void	execute_command_child(t_command *cmd, t_mnsh *shell)
{
	execvp(cmd->command, cmd->args);// proibida
	fprintf(stderr, "minishell: %s: command not found\n", cmd->command);
	handle_exit_code(127);
	free_ast_node(shell->ast_head);
	free_shell(shell);
	exit (handle_exit_code(-1));
}*/

int	execute_and(t_ast_node *node, t_mnsh *shell)
{
	int	left_status;

	left_status = execute_ast(node->left, shell);
	if (left_status == 0)
		return (execute_ast(node->right, shell));
	return (handle_exit_code(left_status));
}

int	execute_or(t_ast_node *node, t_mnsh *shell)
{
	int	left_status;

	left_status = execute_ast(node->left, shell);
	if (left_status != 0)
		return (execute_ast(node->right, shell));
	return (handle_exit_code(left_status));
}

////////////////////////////////////////////////////////////////////////////////

static void	run_left_child(t_ast_node *node, int *pipefd, t_mnsh *shell)
{
	fd_bug("run_left_child", pipefd[0], "closing read end");
	close(pipefd[0]);
	fd_bug("run_left_child", STDOUT_FILENO, "before redirection");
	dup2(pipefd[1], STDOUT_FILENO);
	fd_bug("run_left_child", STDOUT_FILENO, "after redirection");
	fd_bug("run_left_child", pipefd[1], "closing write end after dup2");
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
	fd_bug("run_right_child", pipefd[1], "closing write end");
	close(pipefd[1]);
	fd_bug("run_right_child", STDIN_FILENO, "before redirection");
	dup2(pipefd[0], STDIN_FILENO);
	fd_bug("run_right_child", STDIN_FILENO, "after redirection");
	fd_bug("run_right_child", pipefd[0], "closing read end after dup2");
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

	if (pipe(pipefd) == -1)
		return (perror("minishell: pipe"), 1);
	fd_bug("execute_pipe", pipefd[0], "pipe  read end created");
	fd_bug("execute_pipe", pipefd[1], "pipe write end created");
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
	fd_bug("execute_pipe", pipefd[0], "closing  read end in parent");
	close(pipefd[0]);
	fd_bug("execute_pipe", pipefd[1], "closing write end in parent");
	close(pipefd[1]);
	wait_for_child(left_pid);
	return (handle_exit_code(wait_for_child(right_pid)));
}

////////////////////////////////////////////////////////////////////////////////
