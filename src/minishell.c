/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 13:29:34 by abessa-m          #+#    #+#             */
/*   Updated: 2025/03/18 19:22:35 by abessa-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ad_handle_parse_input(char	*input)
{
	if (DEBUG)
	{
		ft_putstr_fd("--DEBUG--input: ", 1);
		ft_putstr_fd(input, 1);
		write(1, "\n", 1);
	}
	return (0);
}

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
		printf("--DEBUG-- \n--DEBUG-- Hello, friend.\n--DEBUG-- \n");
	else
		printf("Hello, friend.\n");
	while (42)
	{
		input = readline("% ");
		if (input == NULL)
		{
			printf("Bye, dear ;)\n");
			break ;
		}
		else
			ad_handle_parse_input(input);
	}
	return (0);
}
