/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 15:38:09 by alde-alm          #+#    #+#             */
/*   Updated: 2025/04/17 11:15:51 by abessa-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

/*
Iterates through the "envp" array printing each line (string) of the enviroment
variables to the standard output.*/
/*
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

int	main(int ac, char **av, char **envp)
{
	return (ft_env(av, envp));
}
 */
