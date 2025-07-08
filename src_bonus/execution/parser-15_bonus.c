/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser-15.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 18:17:36 by abessa-m          #+#    #+#             */
/*   Updated: 2025/07/03 18:17:48 by abessa-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include_bonus/minishell_bonus.h"

t_exec	*init_pipeline(t_list *pipeline, t_env **env, int prev)
{
	t_exec	*exec;
	int		i;

	exec = ft_malloc(sizeof(*exec) * 1);
	init_exec(pipeline, exec, env, prev);
	i = 0;
	while (pipeline)
	{
		init_cmd(pipeline->content, exec, i);
		pipeline = pipeline->next;
		i++;
	}
	return (exec);
}

void	init_cmd(t_tube *lst, t_exec *exec, int i)
{
	t_cmd	*cmd;

	cmd = &exec->cmds[i];
	cmd->status = 0;
	cmd->full_path = NULL;
	cmd->hd_buffer = NULL;
	cmd->builtin = -1;
	cmd->args = NULL;
	cmd->i = i;
	cmd->n = exec->n;
	cmd->args = extract_args(lst);
	init_redirs(lst, cmd, exec);
	if (cmd->status == 0)
		init_cmd_path(cmd, exec);
}

void	init_redirs(t_tube *lst, t_cmd *cmd, t_exec *exec)
{
	t_tube	*current;

	if (cmd->i == 0)
		cmd->in_fd = STDIN_FILENO;
	else
		cmd->in_fd = exec->pipes[cmd->i - 1][0];
	if (cmd->i == cmd->n - 1)
		cmd->out_fd = STDOUT_FILENO;
	else
		cmd->out_fd = exec->pipes[cmd->i][1];
	current = lst;
	while (current)
	{
		if (update_fd_in(cmd, current, exec) == -1)
			break ;
		if (update_fd_out(cmd, current, exec) == -1)
			break ;
		current = current->next;
	}
}
