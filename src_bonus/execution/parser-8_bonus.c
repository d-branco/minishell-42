/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser-8.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 17:55:11 by abessa-m          #+#    #+#             */
/*   Updated: 2025/07/03 18:32:56 by abessa-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include_bonus/minishell_bonus.h"

int	find_cmd(char **path, char *filename, char **res)
{
	int			errno_value;

	errno_value = ENOENT;
	*res = 0;
	if (ft_strchr(filename, '/') != NULL)
	{
		errno_value = check_file(filename, res);
		if (errno_value != 0)
			print_error(0, filename, strerror(errno_value));
	}
	else
	{
		if (filename[0] == '\0')
			errno_value = ENOENT;
		else
			errno_value = search_path(path, filename, res);
		if (errno_value != 0)
		{
			if (errno_value == ENOENT)
				print_error(0, filename, "command not found");
			else
				print_error(0, filename, strerror(errno_value));
		}
	}
	return (find_cmd_cnt(errno_value));
}

int	find_cmd_cnt(int errno_value)
{
	if (errno_value == 0)
		return (0);
	if (errno_value == ENOENT)
		return (127);
	return (126);
}

int	check_file(char *filename, char **res)
{
	struct stat	path_info;

	*res = 0;
	if (access(filename, F_OK) == -1)
		return (errno);
	if (lstat(filename, &path_info) != 0)
	{
		print_error(0, 0, strerror(errno));
		exit(5);
	}
	if (S_ISDIR(path_info.st_mode))
		return (EISDIR);
	if (access(filename, X_OK) == -1)
		return (errno);
	*res = ft_strdup(filename);
	return (0);
}

int	search_path(char **path, const char *filename, char **res)
{
	int		i;
	int		status;
	int		ret;
	char	*concat;

	ret = ENOENT;
	if (path)
	{
		i = 0;
		while (path[i])
		{
			concat = concat_slash(path[i], filename);
			status = check_file(concat, res);
			free(concat);
			if (status == 0)
				return (0);
			else if (status != ENOENT)
				ret = status;
			i++;
		}
	}
	*res = 0;
	return (ret);
}

char	*concat_slash(const char *str1, const char *str2)
{
	char	*res;
	size_t	buffer_size;

	buffer_size = ft_strlen(str1) + ft_strlen(str2) + 2;
	res = ft_malloc(buffer_size * 1);
	ft_strlcpy(res, str1, buffer_size);
	ft_strlcat(res, "/", buffer_size);
	ft_strlcat(res, str2, buffer_size);
	return (res);
}
