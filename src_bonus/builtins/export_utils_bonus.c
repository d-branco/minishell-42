/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-alm <alde-alm@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 23:37:58 by alde-alm          #+#    #+#             */
/*   Updated: 2025/06/25 23:45:36 by alde-alm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include_bonus/minishell_bonus.h"

static int	create_var_and_value(const char *av,
		char **var, char **val, int append)
{
	const char	*sep;
	int			var_len;

	if (append)
	{
		sep = ft_strnstr(av, "+=", ft_strlen(av));
		var_len = sep - av;
		*var = ft_substr(av, 0, var_len);
		*val = ft_strdup(sep + 2);
	}
	else
	{
		sep = ft_strchr(av, '=');
		var_len = sep - av + 1;
		*var = ft_substr(av, 0, var_len);
		*val = ft_strdup(sep + 1);
	}
	if (!*var || !*val)
		return (free(*var), free(*val), -1);
	return (0);
}

static int	apply_plus_equal(char *var, char *val, char ***envp)
{
	char	*existing;
	char	*new_val;
	char	*key;
	int		ret;

	existing = ft_getenv(*envp, var);
	if (existing)
		new_val = ft_strjoin(existing, val);
	else
		new_val = ft_strdup(val);
	if (!new_val)
		return (free(var), free(val), free(existing), -1);
	key = ft_strjoin(var, "=");
	if (!key)
		return (free(var), free(val), free(existing), free(new_val), -1);
	ret = replace_add_var(key, new_val, envp);
	free(var);
	free(val);
	free(existing);
	free(new_val);
	free(key);
	return (ret);
}

static int	handle_plus_equal(const char *av, char ***envp)
{
	char	*var;
	char	*val;

	if (create_var_and_value(av, &var, &val, 1) == -1)
		return (-1);
	return (apply_plus_equal(var, val, envp));
}

static int	handle_equal(const char *av, char ***envp)
{
	char	*var;
	char	*val;

	if (create_var_and_value(av, &var, &val, 0) == -1)
		return (-1);
	replace_add_var(var, val, envp);
	free(var);
	free(val);
	return (0);
}

int	export_var(const char *av, char ***envp)
{
	if (ft_strnstr(av, "+=", ft_strlen(av)))
		return (handle_plus_equal(av, envp));
	if (ft_strchr(av, '='))
		return (handle_equal(av, envp));
	if (!ft_getenv(*envp, (char *)av))
		add_var_env(ft_strdup(av), envp_size(*envp), envp);
	return (0);
}
