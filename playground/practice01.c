/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   practice01.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 14:43:31 by abessa-m          #+#    #+#             */
/*   Updated: 2025/03/23 17:31:14 by abessa-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(void)
{
	char	*input;

	if (DEBUG)
		printf("--DEBUG-- \n--DEBUG-- Hello, friend.\n--DEBUG-- \n");
	else
		printf("Hello, friend.\n");
	while (42)
	{
		ft_putnbr_fd(ad_exit_code(-1), 1);
		write(1, " ", 1);
		input = readline("% ");
		if ((input == NULL) || (ft_strncmp(input, "exit", 5) == 0))
		{
			if (DEBUG)
				printf("--DEBUG--echooing: %s\n", input);
			printf("	Bye ;)\n");
			break ;
		}
		if ((ft_strncmp(input, "", 1) != 0) && (DEBUG))
			printf("--DEBUG--echooing: %s\n", input);
	}
	return (0);
}
