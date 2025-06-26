/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_util.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-alm <alde-alm@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 14:16:38 by alde-alm          #+#    #+#             */
/*   Updated: 2025/06/05 14:24:22 by alde-alm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	append_and_free(char **dst, char *src)
{
	char	*tmp;

	tmp = ft_strjoin(*dst, src);
	free(*dst);
	*dst = tmp;
	free(src);
}

void	append_char(char **res, char c)
{
	char	tmp[2];

	tmp[0] = c;
	tmp[1] = '\0';
	append_and_free(res, ft_strdup(tmp));
}

char	*get_env_value(const char *name, char **envp)
{
	int		name_len;
	char	*equal_posit;

	name_len = ft_strlen(name);
	while (*envp)
	{
		equal_posit = ft_strchr(*envp, '=');
		if (equal_posit && (equal_posit - *envp == name_len)
			&& ft_strncmp(*envp, name, name_len) == 0)
			return (equal_posit + 1);
		envp++;
	}
	return ("");
}

int	ft_strarr_len(char **arr)
{
	int	i;

	i = 0;
	while (arr && arr[i])
		i++;
	return (i);
}
