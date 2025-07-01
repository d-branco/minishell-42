/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 14:29:16 by alde-alm          #+#    #+#             */
/*   Updated: 2025/07/01 09:23:20 by abessa-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	print_cd_error(const char *path)
{
	ft_dprintf(2, "minishell: cd: %s: ", path);
	perror("");
	handle_exit_code(1);
}

static void	update_pwd_vars(t_mnsh *shell, char *oldpwd)
{
	char	cwd[PATH_MAX];

	if (oldpwd)
		replace_add_var("OLDPWD=", oldpwd, &shell->envp);
	else
	{
		replace_add_var("OLDPWD=", "", &shell->envp);
	}
	if (getcwd(cwd, sizeof(cwd)))
	{
		replace_add_var("PWD=", cwd, &shell->envp);
	}
}

static int	cd_no_args(t_mnsh *shell, char *oldpwd)
{
	char	*home;

	home = ft_getenv(shell->envp, "HOME");
	if (!home)
	{
		ft_dprintf(2, "minishell: cd: HOME not set\n");
		return (free(oldpwd), handle_exit_code(1));
	}
	if (chdir(home) == -1)
	{
		print_cd_error(home);
		free(home);
		return (free(oldpwd), 1);
	}
	free(home);
	update_pwd_vars(shell, oldpwd);
	free(oldpwd);
	return (handle_exit_code(0));
}

static int	cd_with_path(char *path, char *oldpwd, t_mnsh *shell)
{
	if (ft_strcmp(path, "") != 0 && chdir(path) == -1)
	{
		print_cd_error(path);
		return (free(oldpwd), 1);
	}
	update_pwd_vars(shell, oldpwd);
	free(oldpwd);
	return (handle_exit_code(0));
}

int	ft_cd(int argc, char **argv, t_mnsh *shell)
{
	char	cwd[PATH_MAX];
	char	*oldpwd;

	if (argc > 1)
	{
		ft_dprintf(2, "minishell: cd: too many arguments\n");
		return (handle_exit_code(1));
	}
	getcwd(cwd, sizeof(cwd));
	oldpwd = ft_strdup(cwd);
	if (!oldpwd)
		return (handle_exit_code(1));
	if (argc == 0)
		return (cd_no_args(shell, oldpwd));
	else
		return (cd_with_path(argv[0], oldpwd, shell));
}
