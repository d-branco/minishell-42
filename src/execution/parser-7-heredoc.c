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

static char	*expand_line(char *line, t_env *env);
void		heredoc_loop(int write_fd, char *delim, t_env *env);

static void	handle_heredoc_result(int *in_fd, int *pipe_fd, int pid)
{
	int	status;

	close(pipe_fd[1]);
	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		handle_exit_code(130);
		write(STDOUT_FILENO, "\n", 1);
		close(pipe_fd[0]);
		*in_fd = -1;
	}
	else
		*in_fd = pipe_fd[0];
}

void	read_single_heredoc(int *in_fd, char *delim, t_env *env)
{
	int	pipe_fd[2];
	int	pid;

	if (pipe(pipe_fd) == -1)
	{
		perror("pipe");
		exit(1);
	}
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit(1);
	}
	if (pid == 0)
	{
		close(pipe_fd[0]);
		heredoc_loop(pipe_fd[1], delim, env);
		close(pipe_fd[1]);
		exit(0);
	}
	handle_heredoc_result(in_fd, pipe_fd, pid);
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

static char	*expand_line(char *line, t_env *env)
{
	return (param_expansion(line, env, 0));
}

void	heredoc_loop(int write_fd, char *delim, t_env *env)
{
	char	*line;
	char	*expanded;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, delim) == 0)
		{
			free(line);
			break ;
		}
		expanded = expand_line(line, env);
		write(write_fd, expanded, ft_strlen(expanded));
		write(write_fd, "\n", 1);
		free(line);
		free(expanded);
	}
}
