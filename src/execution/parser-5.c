/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser-5.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 17:52:23 by abessa-m          #+#    #+#             */
/*   Updated: 2025/07/03 18:32:38 by abessa-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	command_failure(void)
{
	char	*args[4];

	args[0] = "sh";
	args[1] = "-c";
	args[2] = "exit 127";
	args[3] = NULL;
	execve("/usr/bin/sh", args, NULL);
}

int	start_child(t_exec *exec, int i)
{
	int	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("forking failed");
		exit(4);
	}
	if (pid == 0)
	{
		fd_redirect(exec->cmds[i].in_fd, exec->cmds[i].out_fd);
		close_fds(exec);
		exec_cmd(exec, i);
		close(0);
		close(1);
		if (exec->cmds[i].status == 0)
			exit_ve();
		if (handle_exit_code(-1) == 130)
			heredoc_failure();
		command_failure();
		exit(exec->cmds[i].status);
	}
	return (pid);
}

int	fd_redirect(int input_fd, int output_fd)
{
	dup2(input_fd, STDIN_FILENO);
	dup2(output_fd, STDOUT_FILENO);
	return (0);
}

void	exec_cmd(t_exec *exec, int i)
{
	char	**env_array;

	if (exec->cmds[i].status != 0)
	{
		return ;
	}
	if (exec->cmds[i].builtin != -1)
	{
		exec->cmds[i].status = fd_builtin(exec, i);
	}
	else if (exec->cmds[i].full_path)
	{
		env_array = env_to_strarr(*exec->env);
		execve(exec->cmds[i].full_path, exec->cmds[i].args, env_array);
		print_error(0, exec->cmds[i].full_path, strerror(errno));
		free_strarr(env_array);
		exec->cmds[i].status = 1;
	}
}

void	free_exec(t_exec *exec)
{
	int	i;

	free_pipes(exec->pipes, exec->n - 1);
	free_pipes(exec->hd_pipes, exec->n);
	free_strarr(exec->path);
	i = 0;
	while (i < exec->n)
	{
		free_cmd(&exec->cmds[i]);
		i++;
	}
	free(exec->cmds);
}
