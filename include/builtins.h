/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 14:10:03 by alde-alm          #+#    #+#             */
/*   Updated: 2025/04/17 11:17:24 by abessa-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#define SUCCESS 0
#define ERROR 1
#define NON_EXECUTABLE 126
#define UNKNOWN_COMMAND 127

//# include "libft/libft.h"
//# include <limits.h>
//# include <unistd.h>
//# include <stdio.h>
//# include <stdarg.h>
//# include <stdlib.h>
//# include <string.h>

int	ft_echo(char **av);
int	ft_env(char **av, char **envp);
int	ft_pwd(void);
int	ft_unset(char **av, char ***envp);
int	ft_cd(int ac, char **av, char **envp);
int	ft_exit(int ac, char **av);
