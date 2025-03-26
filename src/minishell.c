/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 13:29:34 by abessa-m          #+#    #+#             */
/*   Updated: 2025/03/26 14:14:33 by abessa-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv)
{
	char	*input;

	(void) argv;
	if (argc > 1)
	{
		ft_putstr_fd("Too many arguments, dear ;)\n", 2);
		return (1);
	}
	if (DEBUG)
		printf("--DEBUG-- \n--DEBUG-- Hello, friend.\n--DEBUG--\n");
	while (42)
	{
		ft_putnbr_fd(ad_exit_code(-1), 1);
		input = readline("% ");
		if ((input == NULL) || (ft_strncmp(input, "exit", 5) == 0))
		{
			ft_putstr_fd("exit\n", 1);
			break ;
		}
		else
			parser(input);
	}
	if (DEBUG)
		printf("--DEBUG-- Goodbye, friend.\n");
	return (ad_exit_code(-1));
}
