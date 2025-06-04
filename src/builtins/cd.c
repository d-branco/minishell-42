/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 14:29:16 by alde-alm          #+#    #+#             */
/*   Updated: 2025/05/14 16:07:13 by abessa-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*ft_getenv(char **envp, char *var_name);

static int	error_cd(const char *path)
{
	if (errno == ENOENT)
		printf("minishell: cd: %s - No such file or directory\n", path);
	else if (errno == EACCES)
		printf("minishell: cd: %s - Permission denied\n", path);
	else if (errno == ENOTDIR)
		printf("minishell: cd: %s - Not a directory\n", path);
	else if (errno == ELOOP)
		printf("minishell: cd: %s - Too many levels of symbolic links\n", path);
	else if (errno == EINVAL)
		printf("minishell: cd: %s - Invalid argument\n", path);
	else if (errno == EFAULT)
		printf("minishell: cd: %s - Bad address\n", path);
	else if (errno == ENAMETOOLONG)
		printf("minishell: cd: %s - File name too long\n", path);
	else
		printf("Unknown cd Error");
	return (1);
}

char	*ft_getenv(char **envp, char *var_name)
{
	int	len;

	len = ft_strlen(var_name);
	while (*envp)
	{
		if (ft_strncmp(*envp, var_name, len) == 0)
			return (ft_strdup(*envp + len));
		envp++;
	}
	return (NULL);
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

int	ft_cd(int ac, char **av, t_mnsh *shell)
{
	char	cwd[PATH_MAX];
	char	*path;
	char	*oldpwd;
	//char	*pwd;
	int		error_code;

	if (ac > 2)
		return (printf("minishell: cd: too many arguments\n"),
			handle_exit_code(1));
	if (!getcwd(cwd, sizeof(cwd)))
		return (handle_exit_code(1));
	shell->export_status = 1;
	if (ac == 1)
	{
		path = ft_getenv(shell->envp, "HOME=");  
		if (!path)
			return (printf("minishell: cd: HOME not set\n"),
				handle_exit_code(1));
	}
	else
		path = av[1];
	if (chdir(path) != 0 && ft_strcmp(path,"") != 0)
	{
		error_code = error_cd(path);
		return (free(path), handle_exit_code(error_code));
	}
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
	if (ac == 1)
		free(path);
	return (handle_exit_code(0));
}
