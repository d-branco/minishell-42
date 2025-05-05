/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-alm <alde-alm@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 12:24:23 by alde-alm          #+#    #+#             */
/*   Updated: 2025/04/23 14:40:51 by alde-alm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	is_builtin(t_command *cmd)
{
	const char	*builtins[8];
	int	i;
	
	builtins[0] = "cd";
	builtins[1] = "echo";
	builtins[2] = "env";
	builtins[3] = "exit";
	builtins[4] = "export";
	builtins[5] = "pwd";
	builtins[6] = "unset";
	builtins[7] = NULL;
	i = -1;
	while (++i < 7)
	{
		if (ft_strlen(builtins[i]) == ft_strlen(cmd->command))
		{
			if (ft_strncmp(builtins[i], cmd->command ,ft_strlen(cmd->command)) == 0)
				return (1);
		}
	}
	return (0);
}

void	execute_builtin(t_command *cmd, t_mnsh *shell)
{
	if (strncmp(cmd->command, "cd", ft_strlen(cmd->command)) == 0)
		ft_cd(cmd->argc, cmd->args, shell->envp);
	else if (strncmp(cmd->command, "echo", ft_strlen(cmd->command)) == 0)
		ft_echo(cmd->args);
	else if (strncmp(cmd->command, "env", ft_strlen(cmd->command)) == 0)
		ft_env(cmd->args, shell->envp);
	else if (strncmp(cmd->command, "exit", ft_strlen(cmd->command)) == 0)
		ft_exit(cmd->argc, cmd->args);
	else if (strncmp(cmd->command, "export", ft_strlen(cmd->command)) == 0)
		ft_export(cmd->args, shell->envp);
	else if (strncmp(cmd->command, "pwd", ft_strlen(cmd->command)) == 0)
		ft_pwd();
	else if(strncmp(cmd->command, "unset", ft_strlen(cmd->command)) == 0)
		ft_unset(cmd->args, shell->envp);
}
