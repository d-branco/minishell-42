/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 15:18:02 by alde-alm          #+#    #+#             */
/*   Updated: 2025/07/03 16:02:40 by abessa-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include_bonus/minishell_bonus.h"

//static void	free_exit_s(t_mnsh *shell)
//{
//	//free_ast_node(shell->ast_head);
//	free_shell(shell);
//}

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

int	exec_exit(char **args, t_env **env, int prev)
{
	int	i;

	i = 0;
	handle_exit_code(prev);
	while (args && *args)
	{
		if (i > 0)
		{
			print_error("exit", 0, "too many arguments");
			return (handle_exit_code(1));
		}
		if (!is_num(*args) || ft_strlen(args[0]) > 19)
		{
			print_error("exit", *args, "numeric argument required");
			env_add(env, "EXIT", "yes");
			return (handle_exit_code(2));
		}
		handle_exit_code(ft_atoi(*args));
		args++;
		i++;
	}
	env_add(env, "EXIT", "yes");
	return (handle_exit_code(-1));
}

/*
int	ft_exit(int ac, char **av, int prev)
{
	write(2, "exit\n", 5);
	if (ac == 0)
	{
		exit(handle_exit_code(prev));
	}
	if (!is_num(av[0]) || ft_strlen(av[0]) > 19)
	{
		ft_dprintf(2, "minishell: exit: %s:	numeric	argument required\n",
			av[0]);
		exit(handle_exit_code(2));
	}
	if (ac == 1)
	{
		handle_exit_code(ft_atoi(av[0]));
		exit(handle_exit_code(-1));
	}
	ft_dprintf(2, "minishell: exit: too many arguments\n");
	return (handle_exit_code(1));
}
*/
