/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 14:29:16 by alde-alm          #+#    #+#             */
/*   Updated: 2025/06/24 23:07:23 by alde-alm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void error_cd(const char *path)
{
	ft_dprintf(2, "minishell: cd: %s: ", path);
	perror("");
}

static void	ft_setenv(char **envp, const char *var_name, const char *str)
{
	while (*envp)
	{
		if (ft_strncmp(*envp, var_name, ft_strlen(var_name)) == 0)
		{
			free(*envp);
			*envp = ft_strjoin(var_name, str);
			if (!*envp)
				return ;
			return ;
		}
		envp++;
	}
}

static int	cd_update_env(t_mnsh *shell)
{
	char	cwd[PATH_MAX];
	char	*oldpwd;

	if (!getcwd(cwd, sizeof(cwd)))
		return (handle_exit_code(1));
	oldpwd = ft_getenv(shell->envp, "PWD=");
	if (oldpwd)
	{
		replace_add_var("OLDPWD=", oldpwd, &shell->envp);
		free(oldpwd);
	}
	else
		ft_setenv(shell->envp, "OLDPWD", "");
	if (getcwd(cwd, sizeof(cwd)))
		ft_setenv(shell->envp, "PWD=", cwd);
	return (handle_exit_code(0));
}

static int	cd_handle_path(int ac, char **av, t_mnsh *shell, char **path_out)
{
	char	*path;

	if (ac == 1)
	{
		path = ft_getenv(shell->envp, "HOME");
		if (!path)
			return (ft_dprintf(2, "minishell: cd: HOME not set\n"),
				handle_exit_code(1));
	}
	else
		path = av[1];
	*path_out = path;
	return (0);
}

int	ft_cd(int ac, char **av, t_mnsh *shell)
{
	char	cwd[PATH_MAX];
	char	*path;

	if (ac > 2)
		return (ft_dprintf(2, "minishell: cd: too many arguments\n"),
			handle_exit_code(1));
	if (!getcwd(cwd, sizeof(cwd)))
		return (handle_exit_code(1));
	shell->export_status = 1;
	if (cd_handle_path(ac, av, shell, &path))
		return (1);
	if (chdir(path) != 0 && ft_strcmp(path, "") != 0)
	{
		error_cd(path);
		if (ac == 1)
			free(path);
		return (handle_exit_code(1));
	}
	if (ac == 1)
		free(path);
	return (cd_update_env(shell));
}
