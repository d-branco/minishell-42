/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-alm <alde-alm@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 15:55:05 by alde-alm          #+#    #+#             */
/*   Updated: 2025/04/12 16:54:40 by alde-alm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	is_var_to_remove(const char *env_var, const char *var_name)
{
	size_t	len;

	len = strlen(var_name);
	return (strncmp(env_var, var_name, len) == 0
		&& (env_var[len] == '=' || env_var[len] == '\0'));
}

static void	remove_env_var(char ***envp, const char *var_name)
{
	int		count;
	int		i;
	int		j;
	char	**new_env;

	i = 0;
	j = 0;
	count = 0;
	while ((*envp)[count])
		count++;
	new_env = ft_malloc(sizeof(char *) * count);
	while (i < count)
	{
		if (is_var_to_remove((*envp)[i], var_name))
		{
			free((*envp)[i]);
			i++;
			continue ;
		}
		new_env[j++] = (*envp)[i++];
	}
	new_env[j] = NULL;
	free(*envp);
	*envp = new_env;
}

int	ft_unset(char **av, char ***envp)
{
	int	i;

	if (!av[1] || !envp || !*envp)
		return (handle_exit_code(0));
	i = 1;
	while (av[i])
	{
		remove_env_var(envp, av[i]);
		i++;
	}
	return (handle_exit_code(0));
}
