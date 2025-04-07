/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 13:46:47 by abessa-m          #+#    #+#             */
/*   Updated: 2025/04/07 12:39:36 by abessa-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	parser(char *input)
{
	t_token	*list_o_tokens;

	list_o_tokens = NULL;
	parse_input_into_token_list(&list_o_tokens, input);
	tkn_lst_printer(list_o_tokens);
	tkn_lstclear(&list_o_tokens);
	return (handle_exit_code(-1));
}
