/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   control_chars.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samy_bravy <samy_bravy@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 14:25:24 by fgrisost          #+#    #+#             */
/*   Updated: 2024/08/31 10:06:08 by samy_bravy       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	spaces(char c)
{
	if ((c == 32 || c == 13 || c == 9 || c == 11))
		return (1);
	return (0);
}

void	special_char(t_input **head, t_token_ctx *ctx)
{
	ctx->i = 0;
	while (ctx->i++ <= 2)
		ctx->temp[ctx->i - 1] = 0;
	ctx->temp[0] = *ctx->copy_str++;
	if ((*ctx->copy_str == '>' || *ctx->copy_str == '<')
		&& ctx->temp[0] == *ctx->copy_str)
		ctx->temp[1] = *ctx->copy_str++;
	ctx->current_type = identify_type(ctx->temp);
	if (ctx->current_type == PIPE)
	{
		if (ctx->cmd_str)
		{
			add_node(head, new_node(CMD, ctx->cmd_str));
			free(ctx->cmd_str);
			ctx->cmd_str = NULL;
		}
		add_node(head, new_node(ctx->current_type, NULL));
	}
}

void	quotes(t_token_ctx *ctx)
{
	ctx->c_for_quotes = *ctx->copy_str;
	ctx->start = ctx->copy_str++;
	while (*ctx->copy_str && *ctx->copy_str != ctx->c_for_quotes)
		ctx->copy_str++;
	ctx->copy_str++;
	ctx->token = ft_strndup(ctx->start, ctx->copy_str - ctx->start);
}

void	normal_char(t_token_ctx *ctx)
{
	ctx->start = ctx->copy_str;
	while (*ctx->copy_str && spaces(*ctx->copy_str) == 0
		&& *ctx->copy_str != '>' && *ctx->copy_str != '<'
		&& *ctx->copy_str != '|')
	{
		if (*ctx->copy_str == '"' || *ctx->copy_str == '\'')
		{
			ctx->c_for_quotes = *ctx->copy_str;
			ctx->copy_str++;
			while (*ctx->copy_str && *ctx->copy_str != ctx->c_for_quotes)
				ctx->copy_str++;
		}
		if (*ctx->copy_str)
			ctx->copy_str++;
	}
	ctx->token = ft_strndup(ctx->start, ctx->copy_str - ctx->start);
}
