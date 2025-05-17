/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 15:18:02 by alde-alm          #+#    #+#             */
/*   Updated: 2025/05/17 14:51:38 by abessa-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_num(char *av)
{
	int	i;

	if (!av)
		return (0);
	i = 0;
	if (av[i] == '+' || av[i] == '-')
		i++;
	while (av[i])
	{
		if (!(av[i] >= '0' && av[i] <= '9'))
			return (0);
		i++;
	}
	return (1);
}

int	ft_exit(int ac, char **av, t_mnsh *shell)
{
	printf("exit\n");
	if (ac == 1)
	{
		free_shell(shell);
		exit(handle_exit_code(0));
	}
	if (ac >= 2 && !is_num(av[1]))
	{
		printf("minishell: exit: %s: numeric argument required\n", av[1]);
		free_shell(shell);
		exit(handle_exit_code(2));
	}
	if (ac >= 2 && is_num(av[1]))
	{
		if (ac == 2)
		{
			free_shell(shell);
			exit(handle_exit_code(ft_atoi(av[1])));
		}
		else
			return (printf("minishell: exit: too many arguments\n"),
				handle_exit_code(1));
	}
	return (handle_exit_code(0));
}
