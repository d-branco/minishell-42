/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser-6-fd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 17:53:33 by abessa-m          #+#    #+#             */
/*   Updated: 2025/07/03 18:32:42 by abessa-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	fd_builtin(t_exec *exec, int i)
{
	int	stdin_clone;
	int	stdout_clone;

	stdin_clone = dup(STDIN_FILENO);
	stdout_clone = dup(STDOUT_FILENO);
	dup2(exec->cmds[i].in_fd, STDIN_FILENO);
	dup2(exec->cmds[i].out_fd, STDOUT_FILENO);
	close_fds(exec);
	exec->cmds[i].status = exec_builtin(exec->cmds[i].builtin,
			exec->cmds[i].args + 1, exec->env, exec->prev);
	dup2(stdin_clone, STDIN_FILENO);
	dup2(stdout_clone, STDOUT_FILENO);
	close(stdin_clone);
	close(stdout_clone);
	return (exec->cmds[i].status);
}

void	close_fds(t_exec *exec)
{
	int	i;

	i = 3;
	while (i < exec->fd_count + 3)
	{
		close(i);
		i++;
	}
}

int	update_fd_out(t_cmd *cmd, t_tube *redir, t_exec *exec)
{
	int	flags;

	if (redir->modifier != e_OUTPUT_REDIRECTION && redir->modifier != e_APPEND)
		return (0);
	flags = O_WRONLY | O_TRUNC | O_CREAT;
	if (redir->modifier == e_APPEND)
		flags = O_WRONLY | O_CREAT | O_APPEND;
	cmd->out_fd = open(redir->word, flags, 0644);
	if (cmd->out_fd < 0)
	{
		print_error(0, redir->word, strerror(errno));
		cmd->status = 1;
		return (-1);
	}
	exec->fd_count++;
	return (0);
}

int	update_fd_in(t_cmd *cmd, t_tube *redir, t_exec *exec)
{
	if (redir->modifier == e_INPUT_REDIRECTION)
	{
		cmd->in_fd = open(redir->word, O_RDONLY);
		if (cmd->in_fd < 0)
		{
			print_error(0, redir->word, strerror(errno));
			cmd->status = 1;
			return (-1);
		}
		exec->fd_count++;
	}
	else if (redir->modifier == e_HERE_DOC)
	{
		read_single_heredoc(&cmd->hd_buffer, redir->word);
		cmd->in_fd = exec->hd_pipes[cmd->i][0];
	}
	return (0);
}
