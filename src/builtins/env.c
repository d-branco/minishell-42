/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-alm <alde-alm@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 15:38:09 by alde-alm          #+#    #+#             */
/*   Updated: 2025/04/07 15:06:19 by alde-alm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
Iterates through the "envp" array printing each line (string) of the enviroment
variables to the standard output.*/

int	ft_env(char **av, char **envp)
{
	int	i;

	if (!envp || !*envp)
		return (SUCCESS);
	if (av[1])
	{
		printf("env: '%s': No such file or directory\n", av[1]);
		return (UNKNOWN_COMMAND);
	}
	i = -1;
	while(envp[++i])
		printf("%s\n", envp[i]);
	return (SUCCESS);
}
