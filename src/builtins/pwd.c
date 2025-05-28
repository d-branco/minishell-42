/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 02:59:19 by alde-alm          #+#    #+#             */
/*   Updated: 2025/05/14 10:39:21 by abessa-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_pwd(void)
{
	char	current_dir[PATH_MAX];

	if (getcwd(current_dir, PATH_MAX))
	{
		printf("%s\n", current_dir);
		return (handle_exit_code(0));
	}
	else
		return (handle_exit_code(1));
}
/*
int	main(void)
{
	return (ft_pwd());
}*/
