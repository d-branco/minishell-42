/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-alm <alde-alm@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 02:59:19 by alde-alm          #+#    #+#             */
/*   Updated: 2025/04/07 14:49:40 by alde-alm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

int	ft_pwd(void)
{
	char	current_dir[PATH_MAX];

	if (getcwd(current_dir, PATH_MAX))
	{
		printf("%s\n", current_dir);
		return (SUCCESS);
	}
	else
		return (ERROR);
}

int	main(void)
{
	return (ft_pwd());
}
