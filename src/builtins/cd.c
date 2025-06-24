/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 14:29:16 by alde-alm          #+#    #+#             */
/*   Updated: 2025/06/24 08:27:49 by abessa-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	error_cd(const char *path)
{
	if (errno == ENOENT)
		printf("Minishell: cd: %s - No such file or directory\n", path);
	else if (errno == EACCES)
		printf("Minishell: cd: %s - Permission denied\n", path);
	else if (errno == ENOTDIR)
		printf("Minishell: cd: %s - Not a directory\n", path);
	else if (errno == ELOOP)
		printf("Minishell: cd: %s - Too many levels of symbolic links\n", path);
	else if (errno == EINVAL)
		printf("Minishell: cd: %s - Invalid argument\n", path);
	else if (errno == EFAULT)
		printf("Minishell: cd: %s - Bad address\n", path);
	else if (errno == ENAMETOOLONG)
		printf("Minishell: cd: %s - File name too long\n", path);
	else
		printf("Unknown cd Error");
	return (1);
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
		path = ft_getenv(shell->envp, "HOME=");
		if (!path)
			return (printf("Minishell: cd: HOME not set\n"),
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
	int		error_code;

	if (ac > 2)
		return (printf("Minishell: cd: too many arguments\n"),
			handle_exit_code(1));
	if (!getcwd(cwd, sizeof(cwd)))
		return (handle_exit_code(1));
	shell->export_status = 1;
	if (cd_handle_path(ac, av, shell, &path))
		return (1);
	if (chdir(path) != 0 && ft_strcmp(path, "") != 0)
	{
		error_code = error_cd(path);
		if (ac == 1)
			free(path);
		return (handle_exit_code(error_code));
	}
	if (ac == 1)
		free(path);
	return (cd_update_env(shell));
}
