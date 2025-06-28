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

int	ft_pwd(void)
{
	char	current_dir[PATH_MAX];

	if (getcwd(current_dir, PATH_MAX))
	{
		printf("%s\n", current_dir);
		return (handle_exit_code(0));
	}
	else
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
