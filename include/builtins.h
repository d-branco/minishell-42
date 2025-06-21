/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 14:10:03 by alde-alm          #+#    #+#             */
/*   Updated: 2025/05/17 13:42:18 by abessa-m         ###   ########.fr       */
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
//src/builtins/pwd.c
int		ft_pwd(void);
//src/builtins/unset.c
int		ft_unset(char **av, char ***envp);
int		is_valid_arg(const char *av);
//src/builtins/check_builtins.c
int		is_builtin(t_command *cmd);
int		execute_builtin(t_command *cmd, t_mnsh *shell);
void	ft_free_env(char **envp);
char	*ft_getenv(char **envp, char *var_name);
int		export_var(const char *av, char ***envp);
//src/builtins/init_utils.c
int		replace_add_var(char *var_name, char *value, char ***envp);
char	**init_envp(char **envp);
void	handle_shlvl(t_mnsh *shell);

#endif
