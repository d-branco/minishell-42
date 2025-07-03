/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 14:29:16 by alde-alm          #+#    #+#             */
/*   Updated: 2025/07/03 14:19:36 by abessa-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
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
*/

static int	update_env(t_env **env);

int	exec_cd(char **args, t_env **env, int prev)
{
	char	*home;
	char	*path;

	(void) prev;
	if (*args)
		path = *args;
	else
	{
		home = ret_env_key(*env, "HOME");
		if (!home || !*home)
			return (print_error("cd", 0, "HOME not set"), ERROR);
		path = home;
	}
	if (chdir(path) != 0)
		return (print_error("cd", path, strerror(errno)), ERROR);
	if (update_env(env) != SUCCESS)
		return (ERROR);
	return (SUCCESS);
}

static int	update_env(t_env **env)
{
	char	newpwd[PATH_MAX + 1];
	char	*oldpwd;

	if (getcwd(newpwd, sizeof(newpwd)) != NULL)
	{
		oldpwd = ft_strdup(ret_env_key(*env, "PWD"));
		env_add(env, "OLDPWD", oldpwd);
		free(oldpwd);
		env_add(env, "PWD", newpwd);
	}
	else
		return (print_error("cd", 0, strerror(errno)), ERROR);
	return (SUCCESS);
}
