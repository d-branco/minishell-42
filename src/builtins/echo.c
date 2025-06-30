/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 16:46:29 by alde-alm          #+#    #+#             */
/*   Updated: 2025/06/30 12:09:28 by abessa-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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

	i = -1;
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
	return (handle_exit_code(0));
}
