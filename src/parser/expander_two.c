/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_two.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alde-alm <alde-alm@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 15:43:15 by alde-alm          #+#    #+#             */
/*   Updated: 2025/06/26 16:02:12 by alde-alm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	handle_dquoted(const char *arg, t_expctx ctx)
{
	int		start;
	char	*literal;

	printf("TTTT\n");
	*(ctx.i) += 2;
	start = *(ctx.i);
	while (arg[*(ctx.i)] && arg[*(ctx.i)] != '"')
		(*(ctx.i))++;
	literal = ft_substr(arg, start, *(ctx.i) - start);
	append_and_free(ctx.res, literal);
	if (arg[*(ctx.i)] == '"')
		(*(ctx.i))++;
	*(ctx.was_quoted) = 1;
}

static void	handle_non_special(const char *arg, t_expctx ctx)
{
	if (arg[*(ctx.i)] == '\'' || arg[*(ctx.i)] == '"')
		handle_quoted(arg, ctx.i, ctx.res, ctx.shell);
	else if (arg[*(ctx.i)] == '$')
		handle_dollar(arg, ctx.i, ctx.res, ctx.shell);
	else
		append_char(ctx.res, arg[(*(ctx.i))++]);
}

static void	handle_segment(const char *arg, t_expctx ctx)
{
	if (arg[*(ctx.i)] == '$' && arg[*(ctx.i) + 1] == '\''
		&& arg[*(ctx.i) + 2] == '\'')
	{
		*(ctx.i) += 3;
		append_and_free(ctx.res, ft_strdup(""));
		*(ctx.was_quoted) = 1;
	}
	else if (arg[*(ctx.i)] == '$' && arg[*(ctx.i) + 1] == '"')
		handle_dquoted(arg, ctx);
	else
	{
		if (arg[*(ctx.i)] == '\'' || arg[*(ctx.i)] == '"')
			*(ctx.was_quoted) = 1;
		handle_non_special(arg, ctx);
	}
}

static void	check_empty_result(char **res, int was_quoted)
{
	if (!was_quoted && (*res)[0] == '\0')
	{
		free(*res);
		*res = NULL;
	}
}

char	*expand_argument(const char *arg, t_mnsh *shell, int *was_quoted_out)
{
	int			i;
	int			was_quoted;
	char		*res;
	t_expctx	ctx;

	i = 0;
	was_quoted = 0;
	res = ft_strdup("");
	ctx = (t_expctx){&i, &res, &was_quoted, shell};
	while (arg[i])
		handle_segment(arg, ctx);
	check_empty_result(&res, was_quoted);
	if (was_quoted_out)
		*was_quoted_out = was_quoted;
	return (res);
}
