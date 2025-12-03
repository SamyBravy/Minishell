/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samy_bravy <samy_bravy@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 14:23:05 by fgrisost          #+#    #+#             */
/*   Updated: 2025/12/03 00:45:00 by samy_bravy       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	check_variable_start_str(t_expand_ctx *ctx)
{
	check_var_len(ctx);
	if (ctx->var_len > 0)
	{
		ctx->var_name = ft_strndup(ctx->str + ctx->var_start, ctx->var_len);
		process_variable_str(ctx);
	}
	else
		ctx->result_len++;
}

size_t	calculate_expand_len(char *str, t_list *env)
{
	t_expand_ctx	ctx;

	init_expand_ctx(&ctx, NULL, env);
	ctx.str = str;
	ctx.len = ft_strlen(str);
	while (ctx.i < ctx.len)
	{
		if (str[ctx.i] == '$')
		{
			ctx.var_start = ctx.i + 1;
			ctx.var_len = 0;
			check_variable_start_str(&ctx);
		}
		else
			ctx.result_len++;
		ctx.i++;
	}
	return (ctx.result_len);
}

static void	copy_value_str(t_expand_ctx *ctx)
{
	size_t	value_len;
	size_t	i;

	value_len = ft_strlen(ctx->var_value);
	i = 0;
	while (i < value_len)
		ctx->result[ctx->result_len++] = ctx->var_value[i++];
}

void	perform_expansion_str(t_expand_ctx *ctx)
{
	ctx->var_name = ft_strndup(ctx->str + ctx->var_start, ctx->var_len);
	ctx->var_value = ft_getenv(ctx->var_name, ctx->env);
	free(ctx->var_name);
	if (ctx->var_value)
		copy_value_str(ctx);
}

void	process_variable_expansion(t_expand_ctx *ctx)
{
	check_var_len(ctx);
	if (ctx->var_len > 0)
	{
		perform_expansion_str(ctx);
		ctx->i += ctx->var_len;
	}
	else
		ctx->result[ctx->result_len++] = ctx->str[ctx->i];
}
