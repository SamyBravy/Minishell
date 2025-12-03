/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_calculate.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samy_bravy <samy_bravy@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 14:31:33 by fgrisost          #+#    #+#             */
/*   Updated: 2025/12/03 00:45:00 by samy_bravy       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	process_char_calc(t_expand_ctx *ctx)
{
	if (ctx->str[ctx->i] == '\'' && ctx->in_double_quotes == -1)
		handle_single_quotes(ctx);
	else if (ctx->str[ctx->i] == '"' && ctx->in_single_quotes == -1)
		handle_double_quotes(ctx);
	else if (ctx->str[ctx->i] == '$' && ctx->in_single_quotes == -1
		&& ctx->current->type != HEREDOC)
		handle_variable_expansion(ctx);
	else
		ctx->result_len++;
}

size_t	calculate_expanded_length(t_input *current, t_list *env)
{
	t_expand_ctx	ctx;

	init_expand_ctx(&ctx, current, env);
	while (ctx.i < ctx.len)
	{
		process_char_calc(&ctx);
		ctx.i++;
	}
	return (ctx.result_len);
}

static void	append_expanded_value(t_expand_ctx *ctx)
{
	ctx->value_len = ft_strlen(ctx->expanded_value);
	ctx->j = 0;
	while (ctx->j < ctx->value_len)
		ctx->result[ctx->result_index++] = ctx->expanded_value[ctx->j++];
}

void	expand_variable_value(t_expand_ctx *ctx)
{
	ctx->var_name = ft_strndup(ctx->str + ctx->var_start, ctx->var_len);
	ctx->var_value = ft_getenv(ctx->var_name, ctx->env);
	if (ctx->current->type != CMD && ctx->var_value == NULL)
		ctx->current->fd = -42;
	free(ctx->var_name);
	if (ctx->var_value)
	{
		if (ctx->in_double_quotes == 1)
			ctx->expanded_value = ft_strdup(ctx->var_value);
		else
			ctx->expanded_value = clean_variable_value(ctx->var_value,
					ctx->current);
		if (ctx->expanded_value)
		{
			append_expanded_value(ctx);
			free(ctx->expanded_value);
		}
	}
	ctx->i += ctx->var_len;
}
