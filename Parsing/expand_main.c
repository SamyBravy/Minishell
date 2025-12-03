/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_main.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samy_bravy <samy_bravy@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 14:33:17 by fgrisost          #+#    #+#             */
/*   Updated: 2025/12/03 00:45:00 by samy_bravy       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	process_expansion(t_expand_ctx *ctx)
{
	ctx->var_start = ctx->i + 1;
	ctx->var_len = 0;
	check_var_len(ctx);
	if (ctx->var_len > 0)
		expand_variable_value(ctx);
	else
		ctx->result[ctx->result_index++] = ctx->str[ctx->i];
}

static void	process_char_expand(t_expand_ctx *ctx)
{
	if (ctx->str[ctx->i] == '\'' && ctx->in_double_quotes == -1)
		ctx->in_single_quotes *= -1;
	else if (ctx->str[ctx->i] == '"' && ctx->in_single_quotes == -1)
		ctx->in_double_quotes *= -1;
	else if (ctx->str[ctx->i] == '$' && ctx->in_single_quotes != 1
		&& ctx->current->type != HEREDOC)
		process_expansion(ctx);
	else
		ctx->result[ctx->result_index++] = ctx->str[ctx->i];
}

char	*expand_variables(t_input *current, t_list *env)
{
	t_expand_ctx	ctx;
	char			*result;

	init_expand_ctx(&ctx, current, env);
	ctx.result_len = calculate_expanded_length(current, env);
	result = malloc(ctx.result_len + 1);
	if (!result)
		return (NULL);
	ctx.result = result;
	while (ctx.i < ctx.len)
	{
		process_char_expand(&ctx);
		ctx.i++;
	}
	result[ctx.result_index] = '\0';
	return (result);
}
