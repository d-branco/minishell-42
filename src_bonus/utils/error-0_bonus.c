/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error-0.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 17:37:15 by abessa-m          #+#    #+#             */
/*   Updated: 2025/07/03 17:37:43 by abessa-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include_bonus/minishell_bonus.h"

void	print_error(char *program, char *arg, char *msg)
{
	int		len;
	char	*prefix;
	char	*buffer;

	prefix = "Minishell";
	len = calc_len(prefix, program, arg, msg);
	buffer = ft_malloc(1 * len);
	ft_strlcpy(buffer, prefix, len);
	ft_strlcat(buffer, ": ", len);
	if (program)
	{
		ft_strlcat(buffer, program, len);
		ft_strlcat(buffer, ": ", len);
	}
	if (arg)
	{
		ft_strlcat(buffer, arg, len);
		ft_strlcat(buffer, ": ", len);
	}
	ft_strlcat(buffer, msg, len);
	ft_strlcat(buffer, "\n", len);
	write(2, buffer, len - 1);
	free(buffer);
}

int	calc_len(char *prefix, char *program, char *arg, char *msg)
{
	int	len;

	len = ft_strlen(prefix) + 2 + ft_strlen(msg) + 2;
	if (program)
		len += ft_strlen(program) + 2;
	if (arg)
		len += ft_strlen(arg) + 2;
	return (len);
}
