/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-alm <alde-alm@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 14:10:03 by alde-alm          #+#    #+#             */
/*   Updated: 2025/04/18 18:57:12 by alde-alm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# define SUCCESS 0
# define ERROR 1
# define NON_EXECUTABLE 126
# define UNKNOWN_COMMAND 127

#include <limits.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

typedef struct	s_mnsh	t_mnsh;

int	ft_echo(char **av);
int	ft_env(char **av, char **envp);
int	ft_pwd(void);
int	ft_unset(char **av, char ***envp);
int	ft_cd(int ac, char **av, char **envp);
int	ft_exit(int ac, char **av);
int	ft_export(char **av, char ***envp);
int	ft_clear(void);
int	replace_add_var(char *var_name, char *value, char ***envp);
int	is_builtin(t_command *cmd);
int	execute_builtin(t_command *cmd, t_mnsh *shell);
char	**init_envp(char **envp);
char	*ft_getenv(char **envp, char *var_name);
void	handle_shlvl(t_mnsh *shell);
void	ft_free_env(char **envp);

#endif
