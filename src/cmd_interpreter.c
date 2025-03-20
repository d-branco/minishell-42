/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_interpreter.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 11:20:04 by abessa-m          #+#    #+#             */
/*   Updated: 2025/03/20 12:30:25 by abessa-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_quotes(char *input);

int	ad_handle_parse_input(char	*input)
{
	if (DEBUG)
	{
		ft_putstr_fd("--DEBUG--INPUT: ", 1);
		ft_putstr_fd(input, 1);
		write(1, "\n", 1);
	}
	ad_exit_code(0);
	//if (check_quotes(input) || check_parenthesis(input) || check_logic(input))
	if (check_quotes(input))
	{
		printf("Oh! There's a syntax error.\n");
		ad_exit_code(2);
	}
	ft_putnbr_fd(ad_exit_code(-1), 1);
	return (ad_exit_code(-1));
}

int	check_quotes(char *input)
{
	int	i;
	int	sum;

	sum = 0;
	i = 0;
	while (input[i])
	{
		if (input[i] == '"')
		sum++;
		i++;
	}
	return (sum % 2);
}

int	ad_exit_code(int newcode)
{
	static int	code = 0;

	if (newcode == -1)
		return (code);
	code = newcode;
	return (code);
}
