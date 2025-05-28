/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 15:12:11 by alde-alm          #+#    #+#             */
/*   Updated: 2025/05/14 10:39:21 by abessa-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	ft_prompt_handler(int signo);
static void	ft_exec_handler(int signo);
/// @brief Configura sinais para modo interativo

void	ft_setup_interactive_signals(void)
{
	struct sigaction	sa;

	sa.sa_handler = ft_prompt_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGTERM, SIG_IGN);
}
/// @brief Trata SIGINT (Ctrl+C) no prompt

static void	ft_prompt_handler(int signo)
{
	if (signo == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
		handle_exit_code(130);
	}
}
/// @brief Configura sinais para processo filho (execução de comando)

void	ft_setup_fork_signals(void)
{
	struct sigaction	sa;

	sa.sa_handler = ft_exec_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}
/// @brief Trata sinais durante execução de comandos (forked)

static void	ft_exec_handler(int signo)
{
	if (signo == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
		handle_exit_code(130);
	}
	else if (signo == SIGQUIT)
	{
		write(STDOUT_FILENO, "Quit\n", 5);
		handle_exit_code(131);
	}
}
