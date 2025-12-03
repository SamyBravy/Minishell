/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wrapper.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samy_bravy <samy_bravy@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 14:23:42 by fgrisost          #+#    #+#             */
/*   Updated: 2025/12/03 00:45:00 by samy_bravy       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	check_dollar_wrapper(t_expand_ctx *ctx)
{
	if (ctx->str[ctx->i] == '$')
	{
		ctx->var_start = ctx->i + 1;
		ctx->var_len = 0;
		process_variable_expansion(ctx);
	}
	else
		ctx->result[ctx->result_len++] = ctx->str[ctx->i];
}

char	*expand_str(char *str, t_list *env)
{
	t_expand_ctx	ctx;
	char			*result;

	init_expand_ctx(&ctx, NULL, env);
	ctx.str = str;
	ctx.len = ft_strlen(str);
	ctx.result_len = calculate_expand_len(str, env);
	result = malloc(ctx.result_len + 1);
	if (!result)
		return (NULL);
	ctx.result = result;
	ctx.result_len = 0;
	while (ctx.i < ctx.len)
	{
		check_dollar_wrapper(&ctx);
		ctx.i++;
	}
	result[ctx.result_len] = '\0';
	return (result);
}
