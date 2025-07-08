/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser-4.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 17:52:01 by abessa-m          #+#    #+#             */
/*   Updated: 2025/07/03 18:32:33 by abessa-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include_bonus/minishell_bonus.h"

//void	write_all_heredocs(t_exec *exec)
//{
//	int	i;
//
//	i = 0;
//	while (i < exec->n)
//	{
//		if (exec->cmds[i].hd_buffer)
//		{
//			write(exec->hd_pipes[i][1],
//				exec->cmds[i].hd_buffer,
//				ft_strlen(exec->cmds[i].hd_buffer));
//			close(exec->hd_pipes[i][1]);
//		}
//		i++;
//	}
//}

void	exit_ve(void)
{
	char	*args[4];

	args[0] = "sh";
	args[1] = "-c";
	args[2] = "exit 0";
	args[3] = NULL;
	execve("/usr/bin/sh", args, NULL);
}

int	wait_all(int n, int *pids)
{
	int	status;
	int	i;

	i = 0;
	while (i < n)
	{
		waitpid(pids[i], &status, WUNTRACED);
		i++;
	}
	return (status);
}

int	get_return_value(int status)
{
	int	exit_code;
	int	sig_num;

	if (WIFEXITED(status))
	{
		exit_code = WEXITSTATUS(status);
		return (exit_code);
	}
	else if (WIFSIGNALED(status))
	{
		sig_num = WTERMSIG(status);
		return (128 + sig_num);
	}
	else
	{
		sig_num = WSTOPSIG(status);
		return (128 + sig_num);
	}
}

////////////////////////////////////////////////////////////////////////////////
int	cmds_with_forks(t_exec *exec)
{
	int			*pids;
	int			ret;

	pids = start_children(exec);
	close_fds(exec);
	ret = wait_all(exec->n, pids);
	free(pids);
	return (get_return_value(ret));
}

int	*start_children(t_exec *exec)
{
	int	*pids;
	int	i;

	pids = ft_malloc(sizeof(*pids) * (exec->n));
	i = 0;
	while (i < exec->n)
	{
		pids[i] = start_child(exec, i);
		i++;
	}
	return (pids);
}
