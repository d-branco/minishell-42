/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 15:18:02 by alde-alm          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2025/06/24 08:27:49 by abessa-m         ###   ########.fr       */
=======
/*   Updated: 2025/06/24 23:15:48 by alde-alm         ###   ########.fr       */
>>>>>>> origin/wildcards
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	free_exit_s(t_mnsh *shell)
{
	//free_ast_node(shell->ast_head);
	free_shell(shell);
}

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

static int	handle_exit(int ac, char **av, t_mnsh *shell)
{
	if (ac == 1)
	{
		free_exit_s(shell);
		exit(handle_exit_code(0));
	}
	if (!is_num(av[1]) || ft_strlen(av[1]) > 19)
	{
<<<<<<< HEAD
		printf("Minishell: exit: %s: numeric argument required\n", av[1]);
=======
		ft_dprintf(2, "minishell: exit: %s:	numeric	argument required\n",
			av[1]);
>>>>>>> origin/wildcards
		free_exit_s(shell);
		exit(handle_exit_code(2));
	}
	if (ac == 2)
	{
<<<<<<< HEAD
		if (ac == 2)
		{
			handle_exit_code(ft_atoi(av[1]));
			free_exit_s(shell);
			exit(handle_exit_code(-1));
		}
		else
			return (printf("Minishell: exit: too many arguments\n"),
				handle_exit_code(1));
=======
		handle_exit_code(ft_atoi(av[1]));
		free_exit_s(shell);
		exit(handle_exit_code(-1));
>>>>>>> origin/wildcards
	}
	ft_dprintf(2, "minishell: exit: too many arguments\n");
	return (handle_exit_code(1));
}

int	ft_exit(int ac, char **av, t_mnsh *shell)
{
	printf("exit\n");
	return (handle_exit(ac, av, shell));
}
