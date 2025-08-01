/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 14:44:01 by alde-alm          #+#    #+#             */
/*   Updated: 2025/06/24 23:21:39 by alde-alm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include_bonus/minishell_bonus.h"

static void	ft_sort_env_tabs(char **tabs);

int	is_valid_arg(const char *av)
{
	int	i;

	i = 1;
	if (!av || (!ft_isalpha(av[0]) && av[0] != '_'))
		return (0);
	while (av[i] && av[i] != '=' && !(av[i] == '+'))
	{
		if (!ft_isalnum(av[i]) && av[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static void	print_export_format(char *env_var)
{
	char	*equal_posit;

	ft_putstr_fd("declare -x ", 1);
	equal_posit = ft_strchr(env_var, '=');
	if (equal_posit)
	{
		*equal_posit = '\0';
		ft_printf("%s=\"%s\"\n", env_var, equal_posit + 1);
		*equal_posit = '=';
	}
	else
	{
		ft_putstr_fd(env_var, 1);
		ft_putchar_fd('\n', 1);
	}
}

static void	print_sort_env(char **envp)
{
	char	**env_copy;
	int		i;

	env_copy = init_expo(envp);
	if (!env_copy)
		return ;
	ft_sort_env_tabs(env_copy);
	i = -1;
	while (env_copy[++i])
		print_export_format(env_copy[i]);
	ft_free_env(env_copy);
}

static void	ft_sort_env_tabs(char **tabs)
{
	int		i;
	int		j;
	char	*tmp;

	if (!tabs)
		return ;
	i = 0;
	while (tabs[i])
	{
		j = i + 1;
		while (tabs[j])
		{
			if (ft_strncmp(tabs[i], tabs[j], ft_strlen(tabs[i]) + 1) > 0)
			{
				tmp = tabs[i];
				tabs[i] = tabs[j];
				tabs[j] = tmp;
			}
			j++;
		}
		i++;
	}
}

int	ft_export(char **av, t_mnsh *shell)
{
	int		i;
	int		status;

	status = 0;
	if (!av[1])
		return (print_sort_env(shell->envp), handle_exit_code(0));
	i = 0;
	while (av[++i])
	{
		if (!is_valid_arg(av[i]))
		{
			ft_dprintf(2, "minishell: export: '%s': not a valid identifier\n",
				av[i]);
			status = 1;
		}
		else if (export_var(av[i], &shell->envp) == -1)
			status = 1;
	}
	return (handle_exit_code(status));
}
