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
	while (*envp)
	{
		if (ft_strncmp(*envp, var_name, ft_strlen(var_name)) == 0)
			//return (ft_strdup(*envp + ft_strlen(var_name)));
			return (*envp + ft_strlen(var_name) + 1);
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
			printf("AQUI 4\n");
			free(*envp);
			*envp = ft_strjoin(var_name, str);
			if (!envp)
				return ;
		}
		envp++;
	}
}

int	ft_cd(int ac, char **av, t_mnsh *shell)
{
	char	cwd[PATH_MAX];
	char	*path;

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
		return (handle_exit_code(error_cd(path)));
	
	if (ft_getenv(shell->envp, "PWD="))
	{
		printf("AQUI 1\n");
		replace_add_var("OLDPWD=", ft_getenv(shell->envp, "PWD="), &shell->envp);
	}
	else
	{
		printf("AQUI 2\n");
		ft_setenv(shell->envp, "OLDPWD", get_env_value("PWD", shell->envp));
	}
	if (getcwd(cwd, sizeof(cwd)) && ft_getenv(shell->envp, "PWD"))
	{
		printf("AQUI 3\n");
		//ft_setenv(shell->envp, "PWD=", cwd);
		replace_add_var("PWD=", cwd, &shell->envp);
	}
	return (handle_exit_code(0));
}
