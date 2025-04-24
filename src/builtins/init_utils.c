/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-alm <alde-alm@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 16:10:29 by alde-alm          #+#    #+#             */
/*   Updated: 2025/04/18 18:59:11 by alde-alm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	add_var_env(char *new_var, int size, char ***envp);

void	ft_free_env(char **envp)
{
	size_t	i;

	if (!envp)
		return ;
	i = 0;
	while (envp[i])
	{
		free(envp[i]);
		i++;
	}
	free(envp);
}

char	**init_envp(char **envp)
{
	char	**new_envp;
	int			count;

	count = 0;
	while (envp[count])
		count++;
	new_envp = ft_calloc((count + 1), sizeof(char *));
	if (!new_envp)
		return (ft_putstr_fd("ERROR malloc failed to alloc memory!\n", 2), NULL);
	count = 0;
	while (envp[count])
	{
		new_envp[count] = ft_strdup(envp[count]);
		if (!new_envp[count])
		{
			ft_free_env(new_envp);
			return (ft_putstr_fd("ERROR malloc failed to alloc memory!\n", 2), NULL);
		}
		count++;
	}	
	new_envp[count] = NULL;
	return (new_envp);
}

void	handle_shlvl(t_mnsh *shell)
{
	char	*lvl;
	char	*new_lvl;
	int	n;

	lvl = ft_getenv(shell->envp, "SHLVL=");
	if (lvl)
	{
		n = ft_atoi(lvl) + 1;
		new_lvl = ft_itoa(n);
		if (!new_lvl)
		{
			ft_putstr_fd("ERROR malloc failed to alloc memory!\n", 2);
			free(lvl);
			return ;
		}
		replace_add_var("SHLVL=", new_lvl, &shell->envp);
		free(new_lvl);
		free(lvl);
	}
	else
		replace_add_var("SHLVL=", "1", &shell->envp);
}

int	replace_add_var(char *var_name, char *value, char ***envp)
{
	char	*new_var;
	int	i;

	if (!*envp || !var_name || !value)
		return (-1);
	new_var = ft_strjoin(var_name, value);
	if (!new_var)
		return (ft_putstr_fd("ERROR malloc failed to alloc memory!\n", 2), -1);
	i = -1;
	while ((*envp)[++i])
	{
		if (ft_strncmp((*envp)[i], var_name, ft_strlen(var_name)) == 0)
		{
			free((*envp)[i]);
			(*envp)[i] = new_var;
			return (SUCCESS);
		}
	}
	return (add_var_env(new_var, i, envp));
}

static int	add_var_env(char *new_var, int size, char ***envp)
{
	char	**new_env;
	int	i;

	if (!new_var || !envp || !*envp)
		return (-1);
	new_env = malloc((size + 2) * sizeof(char*));
	if (!new_env)
	{
		free(new_var);
		return (ft_putstr_fd("ERROR malloc failed to alloc memory!\n", 2), -1);
	}
	i = -1;
	while ((*envp)[++i])
		new_env[i] = (*envp)[i];
	new_env[i] = ft_strdup(new_var);
	new_env[i + 1] = NULL;
	free((*envp));
	*envp = new_env;
	return (SUCCESS);
}
