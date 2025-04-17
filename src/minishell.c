/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 13:29:34 by abessa-m          #+#    #+#             */
/*   Updated: 2025/04/17 15:48:53 by abessa-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv)
{
	char	*input;
	int		loop;

	loop = 42;
	(void) argv;
	if (argc > 1)
	{
		if (argc >= 3 && !ft_strncmp(argv[1], "-c", 3))
			return (handle_exit_code(parser(argv[2])));
		else
			return (ft_putstr_fd("Too many arguments, dear ;)\n", 2), 1);
	}
	if (DEBUG)
		ft_printf("--DEBUG-- \n--DEBUG-- Hello, friend.\n--DEBUG--\n");
	while (loop)
	{
		ft_putnbr_fd(handle_exit_code(-1), 1);
		input = readline("% ");
		if ((input == NULL) || (ft_strncmp(input, "exit", 5) == 0))
		{
			ft_putstr_fd("exit\n", 1);
			break ;
		}
		else
			handle_exit_code(parser(input));
		loop--;
	}
	if (DEBUG)
		ft_printf("--DEBUG-- \n--DEBUG-- Goodbye, friend.\n--DEBUG-- \n");
	return (handle_exit_code(-1));
}

int	handle_exit_code(int newcode)
{
	static int	code = 0;

	if (newcode == -1)
		return (code);
	code = newcode;
	return (code);
}

//		input = "ls -l | grep .c && echo success || (echo"
//			" failure > output.txt && echo -n \"failure but redirected\")";

//char *input = "ls -l | grep .c && echo success || echo failure > output.txt";
//char *input = "a&&b|c||(d&&e&&(f||g))";
