/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils-3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 17:47:10 by abessa-m          #+#    #+#             */
/*   Updated: 2025/07/03 17:47:25 by abessa-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	**env_to_strarr(t_env *env)
{
	char	**ret;
	char	*tmp;
	int		size;
	int		i;

	size = size_env(env);
	ret = ft_malloc(sizeof(*ret) * (size + 1));
	i = 0;
	while (i < size)
	{
		if (env->value == NULL)
			ret[i] = ft_strdup(env->key);
		else
		{
			tmp = ft_strjoin(env->key, "=");
			ret[i] = ft_strjoin(tmp, env->value);
			free(tmp);
		}
		env = env->next;
		i++;
	}
	ret[i] = 0;
	return (ret);
}

size_t	size_env(t_env *env)
{
	size_t	len;

	len = 0;
	while (env != NULL)
	{
		env = env->next;
		len++;
	}
	return (len);
}

void	free_strarr(char **s)
{
	int	i;

	i = 0;
	while (s[i])
		i++;
	while (i >= 0)
	{
		if (s[i])
			free(s[i]);
		i--;
	}
	free(s);
}
