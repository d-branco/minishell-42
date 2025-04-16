/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-alm <alde-alm@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 14:10:03 by alde-alm          #+#    #+#             */
/*   Updated: 2025/04/07 21:14:22 by alde-alm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# define SUCCESS 0
# define ERROR 1
# define NON_EXECUTABLE 126
# define UNKNOWN_COMMAND 127

#include "libft/libft.h"
#include <limits.h>
#include <unistd.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

int	ft_echo(char **av);
int	ft_env(char **av, char **envp);
int	ft_pwd(void);
int	ft_unset(char **av, char ***envp);
int	ft_cd(int ac, char **av, char **envp);
int	ft_exit(int ac, char **av);

#endif
