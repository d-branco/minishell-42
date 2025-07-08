/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 15:38:09 by alde-alm          #+#    #+#             */
/*   Updated: 2025/07/03 16:50:03 by abessa-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include_bonus/minishell_bonus.h"

int	ft_env(char **av, char **envp)
{
	int	i;

	if (!envp || !*envp)
		return (handle_exit_code(0));
	if (av[0])
	{
		ft_dprintf(2, "env: '%s': No such file or directory\n", av[0]);
		return (handle_exit_code(UNKNOWN_COMMAND));
	}
	i = -1;
	while (envp[++i])
	{
		if (!ft_strchr(envp[i], '='))
			continue ;
		printf("%s\n", envp[i]);
	}
	return (handle_exit_code(0));
}

int	exe_env(char **args, t_env **env, int prev)
{
	t_command	*cmd;
	t_mnsh		shell;
	int			ret;

	(void) prev;
	shell.envp = env_to_array(*env);
	cmd = parse_command(args);
	ret = ft_env(cmd->args, shell.envp);
	free_env_array(shell.envp);
	free(cmd);
	return (ret);
}

int	exec_env(char **args, t_env **env, int prev)
{
	char	**strs;

	(void)args;
	(void)prev;
	strs = env_to_strarr(*env);
	strarr_print(strs);
	free_strarr(strs);
	return (0);
}

void	strarr_print(char **s)
{
	int		i;

	if (!*s)
		return ;
	i = -1;
	while (s[++i])
	{
		if (!ft_strchr(s[i], '='))
			continue ;
		ft_printf("%s\n", s[i]);
	}
}
