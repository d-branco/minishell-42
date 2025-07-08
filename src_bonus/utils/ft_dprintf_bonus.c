/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dprintf.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 15:11:45 by alde-alm          #+#    #+#             */
/*   Updated: 2025/06/29 12:57:01 by abessa-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include_bonus/minishell_bonus.h"

static void	putstr_fd(int fd, const char *s)
{
	while (*s)
		write(fd, s++, 1);
}

static void	putnbr_fd(int fd, int n)
{
	char	c;

	if (n == -2147483648)
	{
		putstr_fd(fd, "-2147483648");
		return ;
	}
	if (n < 0)
	{
		write(fd, "-", 1);
		n = -n;
	}
	if (n >= 10)
		putnbr_fd(fd, n / 10);
	c = (n % 10) + '0';
	write(fd, &c, 1);
}

static void	handle_format(int fd, char c, va_list *args)
{
	char	*str;
	int		n;

	if (c == 's')
	{
		str = va_arg(*args, char *);
		if (!str)
			str = "(null)";
		putstr_fd(fd, str);
	}
	else if (c == 'd')
	{
		n = va_arg(*args, int);
		putnbr_fd(fd, n);
	}
	else if (c == '%')
		write(fd, "%", 1);
}

int	ft_dprintf(int fd, const char *format, ...)
{
	va_list		args;
	int			i;

	i = 0;
	va_start(args, format);
	while (format[i])
	{
		if (format[i] == '%' && format[i + 1])
		{
			i++;
			handle_format(fd, format[i], &args);
		}
		else
		{
			write(fd, &format[i], 1);
		}
		i++;
	}
	va_end(args);
	return (0);
}
