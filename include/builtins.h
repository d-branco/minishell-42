/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 14:10:03 by alde-alm          #+#    #+#             */
/*   Updated: 2025/06/26 01:09:38 by alde-alm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# define SUCCESS 0
# define ERROR 1
# define NON_EXECUTABLE 126
# define UNKNOWN_COMMAND 127

# include <limits.h>
# include <errno.h>
# include <unistd.h>
# include <stdio.h>
# include <stdarg.h>
# include <stdlib.h>
# include <string.h>

typedef struct s_mnsh		t_mnsh;
typedef struct s_command	t_command;

//src/builtins/cd.c
int		ft_cd(int ac, char **av, t_mnsh *shell);
//src/builtins/echo.c
int		ft_echo(char **av);
//src/builtins/env.c
int		ft_env(char **av, char **envp);
//src/builtins/exit.c
int		ft_exit(int ac, char **av, t_mnsh *shell);
//src/builtins/export.c
int		ft_export(char **av, t_mnsh *shell);
//src/builtins/export_utils.c
int		export_var(const char *av, char ***envp);
//src/builtins/pwd.c
char	**init_expo(char **envp);
int		ft_pwd(t_mnsh *shell);
//src/builtins/unset.c
int		ft_unset(char **av, char ***envp);
int		is_valid_arg(const char *av);
//src/builtins/replace_add_var.c
int		replace_add_var(char *var_name, char *value, char ***envp);
int		add_var_env(char *new_var, int size, char ***envp);
//src/builtins/check_builtins.c
int		is_builtin(t_command *cmd);
int		envp_size(char **envp);
int		execute_builtin(t_command *cmd, t_mnsh *shell);
void	ft_free_env(char **envp);
char	*ft_getenv(char **envp, char *var_name);
//src/builtins/init_utils.c
char	**init_envp(char **envp);
void	handle_shlvl(t_mnsh *shell);

#endif
