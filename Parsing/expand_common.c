/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_common.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samy_bravy <samy_bravy@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 00:45:00 by samy_bravy        #+#    #+#             */
/*   Updated: 2025/12/03 00:50:00 by samy_bravy       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_terminator(char c)
{
	return (c == '>' || c == '<' || c == '|' || c == '"' || c == '\''
		|| c == '\n' || c == ' ' || c == '$' || c == '/');
}

void	init_expand_ctx(t_expand_ctx *ctx, t_input *current, t_list *env)
{
	ctx->current = current;
	ctx->env = env;
	if (current)
		ctx->str = current->str;
	else
		ctx->str = NULL;
	ctx->i = 0;
	ctx->j = 0;
	ctx->result_len = 0;
	ctx->result_index = 0;
	ctx->in_single_quotes = -1;
	ctx->in_double_quotes = -1;
	if (current)
		ctx->len = ft_strlen(current->str);
	else
		ctx->len = 0;
}

void	check_var_len(t_expand_ctx *ctx)
{
	while (ctx->var_start + ctx->var_len < ctx->len
		&& !is_terminator(ctx->str[ctx->var_start + ctx->var_len]))
	{
		ctx->var_len++;
		if (ctx->str[ctx->var_start] == '?')
			break ;
	}
}

void	process_variable_str(t_expand_ctx *ctx)
{
	ctx->var_value = ft_getenv(ctx->var_name, ctx->env);
	free(ctx->var_name);
	if (ctx->var_value)
		ctx->result_len += ft_strlen(ctx->var_value);
	ctx->i += ctx->var_len;
}

char	*ft_strndup(char *s, size_t n)
{
	char	*src;
	char	*copy;
	size_t	i;

	src = s;
	i = 0;
	if (n < ft_strlen(s))
		copy = malloc(sizeof(char) * (n + 1));
	else
		copy = malloc(sizeof(char) * (ft_strlen(s) + 1));
	if (!copy)
		return (NULL);
	while (src[i] && i < n)
	{
		copy[i] = src[i];
		i++;
	}
	copy[i] = '\0';
	return (copy);
}
