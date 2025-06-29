/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replace_add_var.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-alm <alde-alm@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 01:11:48 by alde-alm          #+#    #+#             */
/*   Updated: 2025/06/26 01:13:31 by alde-alm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	replace_add_var(char *var_name, char *value, char ***envp)
{
	char	*new_var;
	char	*key;
	int		i;

	if (!*envp || !var_name || !value)
		return (-1);
	key = ft_substr(var_name, 0, ft_strchr(var_name, '=') - var_name);
	if (!key)
		return (-1);
	new_var = ft_strjoin(var_name, value);
	if (!new_var)
		return (free(key), ft_putstr_fd("ERROR malloc!\n", 2), -1);
	i = -1;
	while ((*envp)[++i])
	{
		if (ft_strncmp((*envp)[i], key, ft_strlen(key)) == 0
			&& ((*envp)[i][ft_strlen(key)] == '='
				|| (*envp)[i][ft_strlen(key)] == '\0'))
		{
			free((*envp)[i]);
			(*envp)[i] = new_var;
			return (free(key), SUCCESS);
		}
	}
	return (free(key), add_var_env(new_var, i, envp));
}

int	add_var_env(char *new_var, int size, char ***envp)
{
	char	**new_env;
	int		i;

	if (!new_var || !envp || !*envp)
		return (-1);
	new_env = ft_malloc((size + 2) * sizeof(char *));
	i = -1;
	while ((*envp)[++i])
		new_env[i] = (*envp)[i];
	new_env[i] = ft_strdup(new_var);
	new_env[i + 1] = NULL;
	free((*envp));
	free(new_var);
	*envp = new_env;
	return (SUCCESS);
}
