/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 16:55:23 by abessa-m          #+#    #+#             */
/*   Updated: 2025/07/03 17:20:40 by abessa-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include_bonus/minishell_bonus.h"

void	display_ctrl_c(int toggle)
{
	static struct termios	saved_termios;
	static struct termios	modified_termios;

	if (toggle)
	{
		tcgetattr(STDIN_FILENO, &saved_termios);
		tcgetattr(STDIN_FILENO, &modified_termios);
		modified_termios.c_lflag &= ~ECHOCTL;
		tcsetattr(STDIN_FILENO, TCSAFLUSH, &modified_termios);
	}
	else
		tcsetattr(STDIN_FILENO, TCSANOW, &saved_termios);
}

void	sigint_handler(int sig)
{
	if (sig == SIGINT)
	{
		handle_exit_code(130);
		write(STDOUT_FILENO, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
}

void	silent_signal(int sig)
{
	if (sig == SIGQUIT)
	{
	}
	if (sig == SIGINT)
	{
	}
}

void	parent_signals(void)
{
	struct sigaction	sig_quit;
	struct sigaction	sig_int;

	sig_int.sa_handler = &sigint_handler;
	sigemptyset(&sig_int.sa_mask);
	sig_int.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sig_int, NULL);
	sig_quit.sa_handler = SIG_IGN;
	sigemptyset(&sig_quit.sa_mask);
	sig_quit.sa_flags = SA_RESTART;
	sigaction(SIGQUIT, &sig_quit, NULL);
}

//NOTE SIG_ING because this is inherited by a execve process!
void	silent_signals(void)
{
	struct sigaction	sig_int;
	struct sigaction	sig_quit;

	sig_int.sa_handler = &silent_signal;
	sigemptyset(&sig_int.sa_mask);
	sig_int.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sig_int, NULL);
	sig_quit.sa_handler = &silent_signal;
	sigemptyset(&sig_quit.sa_mask);
	sig_quit.sa_flags = SA_RESTART;
	sigaction(SIGQUIT, &sig_quit, NULL);
}
