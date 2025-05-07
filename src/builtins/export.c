/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-alm <alde-alm@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 14:44:01 by alde-alm          #+#    #+#             */
/*   Updated: 2025/04/16 16:04:14 by alde-alm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	ft_sort_env_tabs(char **tabs);

static int	is_valid_arg(const char *av)
{
	int	i;

	i = 1;
	if (!av || (!ft_isalpha(av[0]) && av[0] != '_'))
		return (0);
	while (av[i] && av[i] != '=')
	{
		if (!ft_isalnum(av[i]) && av[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static int	export_var(const char *av, char ***envp)
{
	char	*var_name;
	char	*value;
	char	*equal_posit;

	equal_posit = ft_strchr(av, '=');
	if (equal_posit)
	{
		var_name = ft_substr(av, 0, equal_posit - av + 1);
		value = ft_strdup(equal_posit + 1);
	}
	else
	{
		var_name = ft_strjoin(av, "=");
		value = ft_getenv(*envp, var_name);
		if (!value)
			value = ft_strdup("");
	}
	if (!var_name || !value)
		return (free(var_name), free(value), -1);
	replace_add_var(var_name, value, envp);
	return (free(var_name), free(value), SUCCESS);
}

static void	print_sort_env(char **envp)
{
	char	**env_copy;
	char	*equal_posit;
	int		i;

	env_copy = init_envp(envp);
	if (!env_copy)
		return ;
	ft_sort_env_tabs(env_copy);
	i = -1;
	while (env_copy[++i])
	{
		ft_putstr_fd("declare -x ", 1);
		equal_posit = ft_strchr(env_copy[i], '=');
		if (equal_posit)
		{
			*equal_posit = '\0';
			printf("%s=\"%s\"\n", env_copy[i], equal_posit + 1);
			*equal_posit = '=';
		}
		else
		{
			ft_putstr_fd(env_copy[i], 1);
			ft_putchar_fd('\n', 1);
		}
	}
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

int	ft_export(char **av, char ***envp)
{
	int	i;
	int	status;

	status = 0;
	if (!av[1])
		return (print_sort_env(*envp), SUCCESS);
	i = 1;
	while (av[i])
	{
		if (!is_valid_arg(av[i]))
		{
			printf("Bash: export: '%s': not a valid identifier\n", av[i]);
			status = 1;
		}
		else if (export_var(av[i], envp) == -1)
			status = 1;
		i++;
	}
	return (status);
}
