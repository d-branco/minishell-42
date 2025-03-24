/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 13:29:34 by abessa-m          #+#    #+#             */
/*   Updated: 2025/03/21 16:49:19 by abessa-m         ###   ########.fr       */
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
		printf("--DEBUG-- \n--DEBUG-- Hello, friend.\n--DEBUG-- \n");
	while (42)
	{
		input = readline("% ");
		if (input == NULL)
			break ;
		else
			ad_handle_parse_input(input);
	}

	if (DEBUG)
	printf("asdkjlfnslfnsj\n");
	return (ad_exit_code(-1));
}
