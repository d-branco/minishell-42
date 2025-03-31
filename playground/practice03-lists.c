/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   practice03-lists.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 09:23:00 by abessa-m          #+#    #+#             */
/*   Updated: 2025/03/31 15:23:20 by abessa-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// usage:
//	cc -Wall -Werror -Wextra playground/practice03-lists.c  libft/libft.a -g

#include "../include/minishell.h"

t_s_token	*make_a_token(
					t_e_token_type type, const char *token_string, int length)
{
	t_s_token	*data;

	data = malloc(sizeof(t_s_token));
	if (!data)
		return (NULL);
	data->type = type;
	data->token_string = strdup(token_string);
	data->length = length;
	return (data);
}

//void	print_token_list_info(void *content)
//{
//	t_s_token	*data;
//	const char	*type_strings[] = {"argument(s)", "quote", "length"};
//
//	data = (t_s_token *)content;
//	printf(
//		"Token struct	Token type: %s, Token string: %s, Token length: %d\n",
//		type_strings[data->type], data->token_string, data->length);
//}

void	delete_token(void *content)
{
	t_s_token	*data;

	data = (t_s_token *)content;
	free(data->token_string);
	free(data);
}

void	*iterate_function(void *content)
{
	t_s_token	*old_data;
	t_s_token	*new_data;
	char		modified_token_string[256];

	old_data = (t_s_token *)content;
	new_data = malloc(sizeof(t_s_token));
	new_data->type = old_data->type;
	new_data->length = old_data->length * 2;
	snprintf(modified_token_string, sizeof(modified_token_string),
		"Modified %s", old_data->token_string);
	new_data->token_string = strdup(modified_token_string);
	return (new_data);
}

int	main(void)
{
	t_l_no	*head_node;
	t_l_no	*iterated_list;

	head_node = NULL;
	ft_lstadd_back(&head_node, ft_lstnew(make_a_token(TOKEN_ARG, "echo", 4)));
	ft_lstadd_back(&head_node, ft_lstnew(make_a_token(TOKEN_QUOTE, "", 0)));
	ft_lstadd_back(&head_node, ft_lstnew(make_a_token(TOKEN_QUOTE, "\"", 1)));
	printf("Original List:\n");
	ft_lstiter(head_node, print_token_list_info);
	printf("\nList Size: %d\n", ft_lstsize(head_node));
	iterated_list = ft_lstmap(head_node, iterate_function, delete_token);
	printf("\nList after iterating over every member:\n");
	ft_lstiter(iterated_list, print_token_list_info);
	ft_lstclear(&head_node, delete_token);
	ft_lstclear(&iterated_list, delete_token);
	return (0);
}
