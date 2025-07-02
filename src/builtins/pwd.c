/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 02:59:19 by alde-alm          #+#    #+#             */
/*   Updated: 2025/05/14 10:39:21 by abessa-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_pwd(t_mnsh *shell)
{
	char	*pwd_env;
	char	*cwd;

	pwd_env = ft_getenv(shell->envp, "PWD");
	cwd = getcwd(NULL, 0);
	if (cwd)
	{
		printf("%s\n", cwd);
		free(cwd);
		free(pwd_env);
		return (handle_exit_code(0));
	}
	if (pwd_env)
	{
		printf("%s\n", pwd_env);
		free(pwd_env);
		free(cwd);
		return (handle_exit_code(0));
	}
	ft_dprintf(2, "Minishell: pwd: error current directory\n");
	free(pwd_env);
	return (handle_exit_code(1));
}

char	**init_expo(char **envp)
{
	char	**new_envp;
	int		count;

	count = 0;
	while (envp[count])
		count++;
	new_envp = ft_calloc((count + 1), sizeof(char *));
	if (!new_envp)
		return (ft_putstr_fd("ERROR malloc!\n", 2), NULL);
	count = 0;
	while (envp[count])
	{
		new_envp[count] = ft_strdup(envp[count]);
		if (!new_envp[count])
		{
			ft_free_env(new_envp);
			return (ft_putstr_fd("ERROR malloc!\n", 2), NULL);
		}
		count++;
	}
	new_envp[count] = NULL;
	return (new_envp);
}
