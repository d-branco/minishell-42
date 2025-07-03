/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 13:46:47 by abessa-m          #+#    #+#             */
/*   Updated: 2025/07/03 18:18:21 by abessa-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	parse_n_exec_input(char *input, t_env **all_env)
{
	t_token			*lst_tkn;
	t_token			*lst_tkn_origin;
	t_ast			*ast;

	if (make_tkn_lst(&lst_tkn, input) != 0)
	{
		handle_exit_code(SYNTAX_ERROR);
		return (handle_exit_code(-1));
	}
	lst_tkn_origin = lst_tkn;
	if (parse_ast(&ast, &lst_tkn) != SYNTAX_ERROR)
	{
		handle_exit_code(exec_ast(ast, all_env, handle_exit_code(-1)));
	}
	else
		handle_exit_code(SYNTAX_ERROR);
	free_ast(ast);
	free_lst_tkn(lst_tkn_origin);
	return (handle_exit_code(-1));
}

t_command	*parse_command(char **input)
{
	t_command	*cmd;
	int			count;

	cmd = ft_malloc(sizeof(t_command) * 1);
	count = 0;
	if (input && *input)
	{
		while (input[count])
			count++;
	}
	cmd->command = *input;
	cmd->args = input;
	cmd->argc = count;
	return (cmd);
}

int	exec_builtin(int n, char **args, t_env **env, int prev)
{
	int			ret;
	static int	(*builtin_arr[7])(char **, t_env **, int) = {exec_echo,
		exec_cd, exec_pwd, exec_export, exec_unset, exec_env, exec_exit};

	if (n > 7 || n < 0)
		return (0);
	ret = ((builtin_arr)[n](args, env, prev));
	return (ret);
}

char	*lst_to_str(t_list *word)
{
	char	*ret;
	size_t	size;

	size = 1;
	lst_words_len(word, &size);
	ret = (char *)ft_malloc(sizeof(char) * (size + 1));
	ret[0] = '\0';
	while (word)
	{
		ft_strlcat(ret, word->content, size);
		word = word->next;
	}
	return (ret);
}

void	lst_words_len(t_list *word, size_t *len)
{
	if (!word)
		return ;
	*len += ft_strlen((char *)word->content);
	lst_words_len(word->next, len);
}
