/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-alm <alde-alm@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 14:29:16 by alde-alm          #+#    #+#             */
/*   Updated: 2025/04/18 18:58:45 by alde-alm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*ft_getenv(char **envp, char *var_name);

static int	error_cd(const char *path)
{
	if (errno == ENOENT)
		printf("bash: cd: %s - No such file or directory\n", path);
	else if (errno == EACCES)
		printf("Bash: cd: %s - Permission denied\n", path);
	else if (errno == ENOTDIR)
		printf("Bash: cd: %s - Not a directory\n", path);
	else if (errno == ELOOP)
		printf("Bash:cd : %s - Too many levels of symbolic links\n", path);
	else if (errno == EINVAL)
		printf("Bash:cd : %s - Invalid argument\n", path);
	else if (errno == EFAULT)
		printf("Bash: cd: %s - Bad address\n", path);
	else if (errno == ENAMETOOLONG)
		printf("Bash: cd: %s - File name too long\n", path);
	else
		printf("Unknown cd Error");
	return (ERROR);
}

char	*ft_getenv(char **envp, char *var_name)
{
	while (*envp)
	{
		if (ft_strncmp(*envp, var_name, ft_strlen(var_name)) == 0)
			return (ft_strdup(*envp + ft_strlen(var_name)));
		envp++;
	}
	return (NULL);
}

static void	ft_setenv(char **envp, const char *var_name, const char *cwd)
{
	while (*envp)
	{
		if (ft_strncmp(*envp, var_name, ft_strlen(var_name)) == 0)
		{
			free(*envp);
			*envp = ft_strjoin(var_name, cwd);
			if (!envp)
				return ;
		}
		envp++;
	}
}

int	ft_cd(int ac,char **av, char **envp)
{
	char	cwd[PATH_MAX];
	char	*path;

	if (ac > 2)
	{
		printf("bash: cd: too many arguments\n");
		return (ERROR);
	}
	if (!getcwd(cwd, sizeof(cwd)))
		return (ERROR);
	if (ac == 1)
	{
		path = ft_getenv(envp, "HOME=");
		if (!path)
		{
			printf("bash: cd: HOME not set\n");
			return (ERROR);
		}
	}
	else
		path = av[1];
	if (chdir(path) != 0)
		return (error_cd(path));
	ft_setenv(envp, "OLDPWD=", cwd);
	if (getcwd(cwd, sizeof(cwd)))
		ft_setenv(envp, "PWD=", cwd);
	free(path);
	return (SUCCESS);
}
