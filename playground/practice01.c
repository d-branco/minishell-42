/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   practice01.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 14:43:31 by abessa-m          #+#    #+#             */
/*   Updated: 2025/03/12 16:14:21 by abessa-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(void)
{
	char	*input;

	if (DEBUG)
		printf("Debug mode\n");
	else
		printf("Hello, friend.\n");
	while (42)
	{
		input = readline(">_");
		printf("01 echooing: %s\n", input);
		if ((input == NULL) || (ft_strncmp(input, "exit", 5) == 0))
		{
			printf("01: Bye ;)\n");
			break ;
		}
	}
	return (0);
}
