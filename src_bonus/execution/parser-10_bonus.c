/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser-10.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 18:12:55 by abessa-m          #+#    #+#             */
/*   Updated: 2025/07/03 18:13:16 by abessa-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include_bonus/minishell_bonus.h"

int	**free_pipes(int **pipes, int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		free(pipes[i]);
		i++;
	}
	free(pipes);
	return (0);
}

void	free_cmd(t_cmd *cmd)
{
	free(cmd->full_path);
	free(cmd->hd_buffer);
	free_strarr(cmd->args);
}

void	init_exec(t_list *pipeline, t_exec *exec, t_env **env, int prev)
{
	int		n_pipes;

	exec->n = ft_lstsize(pipeline);
	if (exec->n != 0)
		n_pipes = exec->n - 1;
	else
		n_pipes = 0;
	exec->pipes = get_pipes(n_pipes);
	exec->hd_pipes = get_pipes(exec->n);
	exec->fd_count = n_pipes * 2 + exec->n * 2;
	exec->env = env;
	exec->prev = prev;
	exec->path = extract_path(*env);
	exec->cmds = ft_malloc(sizeof(*(exec->cmds)) * (exec->n));
}

int	**get_pipes(int n)
{
	int	**pipes;
	int	i;

	pipes = ft_malloc(sizeof(*pipes) * n);
	i = 0;
	while (i < n)
	{
		pipes[i] = ft_malloc(sizeof(int) * 2);
		if (pipe(pipes[i]) != 0)
		{
			perror("failed to open pipe");
			exit(3);
		}
		i++;
	}
	return (pipes);
}

char	**extract_path(t_env *env)
{
	int		i;
	char	**env_array;
	char	**ret;

	ret = NULL;
	env_array = env_to_strarr(env);
	if (env_array)
	{
		i = 0;
		while (env_array[i])
		{
			if (ft_strncmp("PATH=", env_array[i], 5) == 0)
			{
				ret = ft_split(env_array[i] + 5, ':');
				break ;
			}
			i++;
		}
	}
	free_strarr(env_array);
	if (ret)
		return (ret);
	return (ft_split("", ':'));
}
