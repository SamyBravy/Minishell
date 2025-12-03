/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_handlers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samy_bravy <samy_bravy@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 14:29:20 by fgrisost          #+#    #+#             */
/*   Updated: 2025/12/03 00:45:00 by samy_bravy       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_double_quotes(t_expand_ctx *ctx)
{
	if (ctx->in_single_quotes == -1)
		ctx->in_double_quotes *= -1;
	else
		ctx->result_len++;
}

void	handle_single_quotes(t_expand_ctx *ctx)
{
	if (ctx->in_double_quotes == -1)
		ctx->in_single_quotes *= -1;
	else
		ctx->result_len++;
}

static void	process_value(t_expand_ctx *ctx)
{
	if (ctx->in_double_quotes == 1)
		ctx->result_len += ft_strlen(ctx->var_value);
	else
	{
		ctx->expanded_value = clean_variable_value(ctx->var_value,
				ctx->current);
		if (ctx->expanded_value)
		{
			ctx->result_len += ft_strlen(ctx->expanded_value);
			free(ctx->expanded_value);
		}
	}
}

void	handle_variable_expansion(t_expand_ctx *ctx)
{
	ctx->var_start = ctx->i + 1;
	ctx->var_len = 0;
	check_var_len(ctx);
	if (ctx->var_len > 0)
	{
		ctx->var_name = ft_strndup(ctx->str + ctx->var_start, ctx->var_len);
		ctx->var_value = ft_getenv(ctx->var_name, ctx->env);
		free(ctx->var_name);
		if (ctx->var_value)
			process_value(ctx);
		ctx->i += ctx->var_len;
	}
	else
		ctx->result_len++;
}
