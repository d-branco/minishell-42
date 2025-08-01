/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 16:10:29 by alde-alm          #+#    #+#             */
/*   Updated: 2025/06/26 01:12:45 by alde-alm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include_bonus/minishell_bonus.h"

static int	count_and_copy_env(char **envp, char **new_envp)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (envp[i])
	{
		if (ft_strchr(envp[i], '='))
		{
			if (new_envp)
			{
				new_envp[j] = ft_strdup(envp[i]);
				if (!new_envp[j])
					return (0);
			}
			j++;
		}
		i++;
	}
	if (new_envp)
		new_envp[j] = NULL;
	return (j);
}

char	**init_envp(char **envp)
{
	char	**new_envp;
	int		count;

	count = count_and_copy_env(envp, NULL);
	new_envp = ft_calloc(count + 1, sizeof(char *));
	if (!new_envp)
		return (ft_putstr_fd("ERROR malloc!\n", 2), NULL);
	if (!count_and_copy_env(envp, new_envp))
	{
		ft_free_env(new_envp);
		ft_putstr_fd("ERROR malloc!\n", 2);
		return (NULL);
	}
	return (new_envp);
}

static bool	is_valid_num(char *str)
{
	if (!str || !*str)
		return (false);
	if (*str == '-')
		str++;
	if (*str == '+')
		str++;
	while (*str)
	{
		if (!ft_isdigit(*str))
			return (false);
		str++;
	}
	return (true);
}

static char	*shlvl_to_str(int n)
{
	if (n > 999)
	{
		ft_printf("minishell: warning: shell level (%d) too high, "
			"resetting to 1\n", n);
		return (ft_strdup("1"));
	}
	else if (n < 0)
		return (ft_strdup("0"));
	return (ft_itoa(n));
}

void	handle_shlvl(t_mnsh *shell)
{
	char	*lvl;
	char	*new_lvl;
	int		n;

	lvl = ft_getenv(shell->envp, "SHLVL");
	if (lvl && is_valid_num(lvl))
	{
		n = ft_atoi(lvl) + 1;
		new_lvl = shlvl_to_str(n);
		if (!new_lvl)
			return (ft_putstr_fd("ERROR malloc!\n", 2), free(lvl), (void)0);
		replace_add_var("SHLVL=", new_lvl, &shell->envp);
		free(lvl);
		free(new_lvl);
	}
	else
	{
		new_lvl = ft_strdup("1");
		if (!new_lvl)
			return (ft_putstr_fd("ERROR malloc!\n", 2), (void)0);
		replace_add_var("SHLVL=", new_lvl, &shell->envp);
		free(new_lvl);
	}
}
