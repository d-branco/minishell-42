/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   practice01.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 14:43:31 by abessa-m          #+#    #+#             */
/*   Updated: 2025/03/12 20:21:57 by abessa-m         ###   ########.fr       */
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
		input = readline(">_");
		if ((input == NULL) || (ft_strncmp(input, "exit", 5) == 0))
		{
			printf("01 echooing: %s\n01: Bye ;)\n", input);
			break ;
		}
		if (ft_strncmp(input, "", 1) != 0)
			printf("01 echooing: %s\n", input);
	}
	return (0);
}
