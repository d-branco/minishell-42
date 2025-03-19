/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_interpreter.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 11:20:04 by abessa-m          #+#    #+#             */
/*   Updated: 2025/03/19 18:25:18 by abessa-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ad_handle_parse_input(char	*input)
{
	if (DEBUG)
	{
		ft_putstr_fd("--DEBUG--INPUT: ", 1);
		ft_putstr_fd(input, 1);
		write(1, "\n", 1);
	}
	ft_putnbr_fd(ad_exit_code(-1), 1);
	return (0);
}

int	ad_exit_code(int newcode)
{
	static int	code = 0;

	if (newcode == -1)
		return (code);
	code = newcode;
	return (code);
}
