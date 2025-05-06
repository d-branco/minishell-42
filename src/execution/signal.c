/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-alm <alde-alm@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 15:12:11 by alde-alm          #+#    #+#             */
/*   Updated: 2025/05/06 15:52:42 by alde-alm         ###   ########.fr       */
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

	sigaction(SIGINT, &sa, NULL);	// Ctrl+C → nova linha
	signal(SIGQUIT, SIG_IGN);		// Ctrl+\ → ignorado
	signal(SIGTERM, SIG_IGN);		// kill -TERM → ignorado
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
		g_exit = 130; // ou EXIT_SIGINT
	}
}
/// @brief Configura sinais para processo filho (execução de comando)
void	ft_setup_fork_signals(void)
{
	struct sigaction	sa;

	sa.sa_handler = ft_exec_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;

	sigaction(SIGINT, &sa, NULL);   // Ctrl+C → imprime \n
	sigaction(SIGQUIT, &sa, NULL);  // Ctrl+\ → imprime Quit
}
/// @brief Trata sinais durante execução de comandos (forked)
static void	ft_exec_handler(int signo)
{
	if (signo == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
		g_exit = 130; // ou EXIT_SIGINT
	}
	else if (signo == SIGQUIT)
	{
		write(STDOUT_FILENO, "Quit\n", 5);
		g_exit = 131; // ou EXIT_SIGQUIT
	}
}
