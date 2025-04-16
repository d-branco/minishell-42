/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-alm <alde-alm@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 14:29:16 by alde-alm          #+#    #+#             */
/*   Updated: 2025/04/13 19:46:14 by alde-alm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

char	*ft_getenv(char **envp, char *var_name)
{
	while (*envp)
	{
		if (ft_strncmp(*envp, var_name, ft_strlen(var_name)) == 0)
			return (strdup(*envp + ft_strlen(var_name)));
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
			printf("%s\n\n", *envp);
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
		printf("\n\n");
		printf("av[2]: %s\n", av[2]);
		printf("bash: cd: too many arguments\n");
		return (ERROR);
	}
	if (!getcwd(cwd, sizeof(cwd)))
		return (ERROR);
	printf("cwd: %s\n\n", cwd);
	if (!av[1])
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
	printf("path: %s\n\n", av[1]);
	if (chdir(path) == -1)
	{
		printf("bash: cd: %s\n", path);
		return (-1);
	}
	ft_setenv(envp, "OLDPWD=", cwd);
	if (getcwd(cwd, sizeof(cwd)))
		ft_setenv(envp, "PWD=", cwd);
	//free(path);
	return (SUCCESS);
}

char	**ft_new_envp(char **envp)
{
	char	**new_envp;
	int		count;

	count = 0;
	while (envp[count])
		count++;
	new_envp = (char **)malloc(sizeof(char *) * (count + 1));
	if (!new_envp)
		return (NULL);
	count = 0;
	while (envp[count])
	{
		new_envp[count] = ft_strdup(envp[count]);
		count++;
	}
	new_envp[count] = NULL;
	return (new_envp);
}

int	main(int ac, char **av, char **envp)
{
	if (ac < 1)
		return (1);
	char **new_envp;

	new_envp = ft_new_envp(envp);
	if (!new_envp)
		return (1);
	int	i = 0;
	while (new_envp[i])
	{
		printf("%s -- %d\n", new_envp[i], i);
		i++;
	}
	printf("\n\n");
	ft_cd(ac, av, new_envp);
	printf("\n\n");
	i = 0;
	while (new_envp[i])
	{
		printf("%s -- %d\n", new_envp[i], i);
		i++;
	}
//	ft_free_vector(new_envp);
	return (0);
}
