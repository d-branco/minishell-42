/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser-7-heredoc.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 17:54:11 by abessa-m          #+#    #+#             */
/*   Updated: 2025/07/03 18:32:48 by abessa-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	read_single_heredoc(char **buffer, char *delim)
{
	char	*line;

	free(*buffer);
	*buffer = ft_strdup("");
	while (TRUE)
	{
		line = get_input_line("> ");
		if (line == NULL || ft_strcmp(line, delim) == 0)
		{
			free(line);
			break ;
		}
		add_line(buffer, line);
	}
}

void	add_line(char **buffer, char *line)
{
	char	*tmp;
	int		size;

	size = ft_strlen(line) + 2;
	if (*buffer)
		size += ft_strlen(*buffer);
	tmp = ft_malloc(sizeof(*tmp) * size);
	if (*buffer)
	{
		ft_strlcpy(tmp, *buffer, size);
		ft_strlcat(tmp, line, size);
	}
	else
		ft_strlcpy(tmp, line, size);
	tmp[size - 2] = '\n';
	tmp[size - 1] = '\0';
	free(*buffer);
	free(line);
	*buffer = tmp;
}

char	*get_input_line(char *prompt)
{
	char	*line;

	if (isatty(0) && isatty(2))
		line = readline(prompt);
	else
	{
		line = get_next_line(0);
		remove_endl(line);
	}
	return (line);
}

void	remove_endl(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (line[i] == '\n')
		{
			line[i] = '\0';
			break ;
		}
		i++;
	}
}
