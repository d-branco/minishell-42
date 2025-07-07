/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 13:29:34 by abessa-m          #+#    #+#             */
/*   Updated: 2025/07/07 19:24:13 by abessa-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	main(int argc, char **argv, char **envp)
{
	t_mnsh			*shell;
	static t_env	*env[1];

	if (isatty(0) && isatty(2))
		rl_outstream = stderr;
	handle_args(argc, argv);
	shell = ft_malloc(sizeof(t_mnsh) * 1);
	display_ctrl_c(TRUE);
	parent_signals();
	signal(SIGPIPE, SIG_IGN);
	init_shell(shell, envp);
	*env = make_ll_env(shell->envp);
	free_shell(shell);
	check_args(argc, argv);
	looping_shell(env);
	rl_clear_history();
	free_all_env(*env);
	return (handle_exit_code(-1));
}

void	check_args(int argc, char **argv)
{
	int	fd;

	if (argc > 1)
	{
		fd = open(argv[1], O_RDONLY);
		if (fd < 0)
		{
			print_error(0, argv[1], strerror(errno));
			if (errno == ENOENT)
				exit(127);
			else
				exit(126);
		}
		dup2(fd, 0);
		close(fd);
	}
}

void	looping_shell(t_env **env)
{
	char	*input;

	while (TRUE)
	{
		input = readline("% ");
		if (input == NULL)
			break ;
		if (ft_check_input(input))
			add_history(input);
		silent_signals();
		display_ctrl_c(FALSE);
		if (input[0])
			handle_exit_code(parse_n_exec_input(input, env));
		if (*ret_env_key(*env, "EXIT"))
			break ;
		display_ctrl_c(TRUE);
		parent_signals();
	}
	if (isatty(0) && isatty(2))
		ft_putstr_fd("exit\n", STDERR_FILENO);
	display_ctrl_c(FALSE);
}

int	handle_exit_code(int newcode)
{
	static int	code = 0;

	if (newcode == -1)
		return (code);
	code = newcode;
	return (code);
}
