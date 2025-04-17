/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-alm <alde-alm@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 15:18:02 by alde-alm          #+#    #+#             */
/*   Updated: 2025/04/07 22:07:16 by alde-alm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

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
//(int ac, char **av, t_shell *sh)
//implementar a função ft_end_free_sh()
int	ft_exit(int ac, char **av)
{
	printf("exit\n");
	if (ac == 1)
	{
		//ft_end_free_sh();
		exit(0);
	}
	if (ac >= 2 && !is_num(av[1]))
	{
		printf("bash: exit: %s: numeric argument required\n", av[1]);
		//ft_end_free_sh();
		exit(2);
	}
	if (ac >= 2 && is_num(av[1]))
	{
		if (ac == 2)
		{
		//ft_end_free_sh();
			exit(ft_atoi(av[1]));
		}
		else
		{
			printf("bash: exit: too many arguments\n");
			return (ERROR);
		}
	}
	return (SUCCESS);
}

int	main(int ac, char **av)
{
	return (ft_exit(ac, av));
}
