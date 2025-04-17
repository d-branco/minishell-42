/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 16:46:29 by alde-alm          #+#    #+#             */
/*   Updated: 2025/04/17 11:16:59 by abessa-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
/*
static int	n_flag(char *str)
{
	int	i;

	i = 0;
	if (str[i] != '-')
		return (0);
	i++;
	while (str[i] && str[i] == 'n')
		i++;
	if (!str[i])
		return (1);
	return (0);
}

int	ft_echo(char **av)
{
	int	i;
	int	nflag;

	i = 0;
	nflag = '\n';
	while (av[++i] && n_flag(av[i]))
		nflag = '\0';
	while (av[i])
	{
		printf("%s", av[i]);
		if (av[i + 1])
			printf(" ");
		i++;
	}
	printf("%c", nflag);
	return (SUCCESS);
}

int	main()
{
	//char *av[] = {"echo", "-nf", "Hello", "World", NULL};
	//ft_echo(av);
	//char *av1[] = {"echo", "-nnnnn", "-n", "Hello", "-nn", "World", NULL};
	//ft_echo(av1);
	//char *av2[] = {"echo", "-nnnnnnnnnnnnnnnnnn",
				"-nnnnnnnnnnnnnnnunnnnnn", "Hello", "World", NULL};
	//ft_echo(av2);
	char *av3[] = {"echo", "$USER", NULL};
	ft_echo(av3);
}
 */
