/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 13:46:47 by abessa-m          #+#    #+#             */
/*   Updated: 2025/06/12 10:30:34 by abessa-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int		parse_n_exec_input(char *input, t_mnsh *shell);

t_env	*make_ll_env(char **envp);
void	env_add(t_env **env, char *key, char *value);
t_env	*make_env(char *key, char *value, t_env *next);
void	free_all_env(t_env *env);
void	free_ll_env(t_env *env);

int		make_tkn_lst(t_token **lst, char *str);
void	tkn_free_lst(t_token *lst);
void	tkn_free_one(t_token *tkn);
void	next_token(t_token **list);



int	parse_n_exec_input(char *input, t_mnsh *shell)
{
	t_token			*lst_tkn;
	t_token			*lst_tkn_origin;
	t_ast_node		*ast;
	static t_env	*ll_env[1];

	if (make_lst_tkn(&lst_tkn, input) != 0)
	{
		handle_exit_code(1);
		return (handle_exit_code(-1));
	}
	lst_tkn_origin = lst_tkn;
	if (parse_tree(&ast, &lst_tkn) != SYNTAX_ERROR)
	{
		*ll_env = make_ll_env(shell->envp);
		handle_exit_code(exec_tree(ast, ll_env));
		free_all_env(*ll_env);
	}
	else
		handle_exit_code(1);
	tree_cleanup(ast);
	lst_tkn_cleanup(lst_tkn_origin);
}

////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
int	make_tkn_lst(t_token **lst, char *str)
{
	t_token	*end;
	t_token	*tmp;

	*lst = NULL;
	while (1)
	{
		if (lexer(&tmp, &str) != 0)
		{
			tkn_free_one(tmp);
			tkn_free_lst(*lst);
			return (-1);
		}
		if (*lst == NULL)
		{
			*lst = tmp;
			end = *lst;
		}
		else
		{
			end->next = tmp;
			end = end->next;
		}
		if (tmp->type == e_END)
			return (0);
	}
}

void	tkn_free_lst(t_token *lst)
{
	t_token	*previous;

	previous = 0;
	while (lst)
	{
		tkn_free_one(previous);
		previous = lst;
		lst = lst->next;
	}
	tkn_free_one(previous);
}

void	tkn_free_one(t_token *tkn)
{
	if (tkn)
		free(tkn->str);
	free(tkn);
}

void	next_token(t_token **list)
{
	*list = (*list)->next;
}

////////////////////////////////////////////////////////////////////////////////
t_env	*make_ll_env(char **envp)
{
	t_env	*ptr;
	char	*key;
	char	*value;
	int		index;

	ptr = NULL;
	while (envp && *envp)
	{
		index = ft_strchr(*envp, '=') - *envp;
		key = ft_substr(*envp, 0, index);
		value = ft_substr(*envp, index + 1, ft_strlen(*envp + index));
		env_add(&ptr, key, value);
		free(key);
		free(value);
		envp++;
	}
	return (ptr);
}

void	env_add(t_env **env, char *key, char *value)
{
	t_env	*current;
	t_env	**ptr;
	t_env	*new_node;

	if (!*key)
		return ;
	ptr = env;
	current = *env;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			free(current->value);
			current->value = ft_strdup(value);
			break ;
		}
		ptr = &current->next;
		current = current->next;
	}
	if (current == NULL)
	{
		new_node = make_env(ft_strdup(key), ft_strdup(value), NULL);
		*ptr = new_node;
	}
}

t_env	*make_env(char *key, char *value, t_env *next)
{
	t_env	*new;

	new = (t_env *)ft_malloc(sizeof(t_env) * 1);
	new->key = key;
	new->value = value;
	new->next = next;
	return (new);
}

void	free_all_env(t_env *env)
{
	t_env	*previous;

	previous = 0;
	while (env)
	{
		free_ll_env(previous);
		previous = env;
		env = env->next;
	}
	free_ll_env(previous);
}

void	free_ll_env(t_env *env)
{
	if (env)
	{
		free(env->key);
		free(env->value);
	}
	free(env);
}

////////////////////////////////////////////////////////////////////////////////

/*int	parser(char *input, t_mnsh *shell)
{
	t_token		*list_o_tokens;
	t_token		*list_o_tokens_origin;
	t_ast_node	*ast;

	if (validate_syntax(input) == SYNTAX_ERROR)
	{
		printf("Minishell: Syntax error: %s\n", input);
		return (handle_exit_code(2));
	}
	list_o_tokens = NULL;
	parse_input_into_token_list(&list_o_tokens, input);
	lst_tkn_printer(list_o_tokens);
	list_o_tokens_origin = list_o_tokens;
	ast = build_ast(&list_o_tokens);
	lst_tknclear(&list_o_tokens_origin);
	if (DEBUG)
		print_ast(ast, 0);
	shell->ast_head = ast;
	execute_ast(shell->ast_head, shell);
	free_ast_node(shell->ast_head);
	return (handle_exit_code(-1));
}*/

//	Parsing order:
//1. Logic operators	||, &&
//2. Pipes				|
//3. Redirections		<, <<, >, >>
//4. Commands			et cetera
