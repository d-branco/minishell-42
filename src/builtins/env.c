/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 15:38:09 by alde-alm          #+#    #+#             */
/*   Updated: 2025/05/14 10:39:21 by abessa-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_env(char **av, char **envp)
{
	int	i;

	if (!envp || !*envp)
		return (handle_exit_code(0));
	if (av[1])
	{
		printf("env: '%s': No such file or directory\n", av[1]);
		return (handle_exit_code(UNKNOWN_COMMAND));
	}
	i = -1;
	while (envp[++i])
		printf("%s\n", envp[i]);
	return (handle_exit_code(0));
}
