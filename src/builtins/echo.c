/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 16:46:29 by alde-alm          #+#    #+#             */
/*   Updated: 2025/07/03 16:49:09 by abessa-m         ###   ########.fr       */
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
		ft_printf("%s", av[i]);
		if (av[i + 1])
			ft_printf(" ");
		i++;
	}
	ft_printf("%c", nflag);
	return (handle_exit_code(0));
}

int	exec_echo(char **args, t_env **env, int prev)
{
	(void) env;
	(void) prev;
	return (ft_echo(args));
}
