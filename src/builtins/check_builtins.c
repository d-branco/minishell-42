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

int	check_builtin(char **cmd)
{
	char	*builtins[8];
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
		if (ft_strlen(builtins[i]) == ft_strlen(cmd[0]))
		{
			if (ft_strncmp(builtins[i], cmd[0] ,ft_strlen(cmd[0])) == 0)
				return (1);
		}
	}
	return (0);
}

int	is_builtin(char **av, t_mnsh *shell)
{
	int	i;
	
	if (check_builtin(av))
	{
		if (strncmp(av[0], "cd", 2) == 0)
			ft_cd(ac, av, shell);
		else if (strncmp(av[0], "echo", 4) == 0)
			ft_echo(av);
		else if (strncmp(av[0], "env", 3) == 0)
			ft_env(av, shell);
		else if (strncmp(av[0], "exit", 4) == 0)
			ft_exit(ac, av);
		else if (strncmp(av[0], "export", 5) == 0)
			ft_export(av, shell);
		else if (strncmp(av[0], "pwd", 3) == 0)
			ft_pwd(void);
		else
			ft_unset(av, shell);
	}
}
