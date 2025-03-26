/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 13:46:47 by abessa-m          #+#    #+#             */
/*   Updated: 2025/03/26 14:33:29 by abessa-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
	syntax error
	Deal with normal command
			normal commands + arguments
	Redirections <, <<, > and >>
	Pipelines |
	Logical operators && and ||
*/

static int	check_logic_operator_existence(char *input);

void	parser(char *input)
{
	if (DEBUG)
		printf("--DEBUG--input to parse: %s\n", input);
	if (check_logic_operator_existence(input))
	{
		if (DEBUG)
			printf("--DEBUG--There are lofig operators\n");
	}
	else
		execute_cmd(input);
}

static int	check_logic_operator_existence(char *input)
{
	int	i;

	i = 0;
	while (input[i])
	{
		if ((input[i] == '&' && input[i + 1] == '&')
			|| (input[i] == '|' && input[i + 1] == '|'))
			return (TRUE);
		i++;
	}
	return (FALSE);
}
