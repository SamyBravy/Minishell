/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samy_bravy <samy_bravy@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 14:26:52 by fgrisost          #+#    #+#             */
/*   Updated: 2024/08/31 10:06:08 by samy_bravy       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	save_cmd(t_token_ctx *ctx)
{
	char	*help;

	if (ctx->cmd_str)
	{
		help = ctx->copy_str - ft_strlen(ctx->token) - 1;
		ctx->temp_cmd_str = calloc(ft_strlen(ctx->cmd_str)
				+ ft_strlen(ctx->token) + 2, 1);
		ft_strlcpy(ctx->temp_cmd_str, ctx->cmd_str, INT_MAX);
		if (spaces(*help) == 1)
			ft_strlcat(ctx->temp_cmd_str, "\x1D", INT_MAX);
		ft_strlcat(ctx->temp_cmd_str, ctx->token, INT_MAX);
		free(ctx->cmd_str);
		ctx->cmd_str = ctx->temp_cmd_str;
	}
	else
		ctx->cmd_str = ft_strndup(ctx->token, INT_MAX);
}

static void	save_special(t_input **head, t_token_ctx *ctx)
{
	if (ctx->cmd_str)
	{
		add_node(head, new_node(CMD, ctx->cmd_str));
		free(ctx->cmd_str);
		ctx->cmd_str = NULL;
	}
	add_node(head, new_node(ctx->current_type, ctx->token));
	ctx->current_type = CMD;
}

static void	save_not_pipe(t_input **head, t_token_ctx *ctx)
{
	if (*ctx->copy_str == '"' || *ctx->copy_str == '\'')
		quotes(ctx);
	else
		normal_char(ctx);
	if (ctx->current_type == CMD || ctx->current_type == PIPE)
		save_cmd(ctx);
	else
		save_special(head, ctx);
	free(ctx->token);
}

static t_token_ctx	*prepare_var(char *str)
{
	t_token_ctx	*ctx;

	ctx = malloc(sizeof(t_token_ctx));
	ctx->token = NULL;
	ctx->current_type = CMD;
	ctx->cmd_str = NULL;
	ctx->copy_str = str;
	ctx->flag = 0;
	return (ctx);
}

t_input	*tokenize(char *str)
{
	t_input				*head;
	t_token_ctx			*ctx;

	head = NULL;
	ctx = prepare_var(str);
	while (*ctx->copy_str)
	{
		while (spaces(*ctx->copy_str) == 1)
			ctx->copy_str++;
		if (*ctx->copy_str == '\0')
			break ;
		if (*ctx->copy_str == '>' || *ctx->copy_str == '<'
			|| *ctx->copy_str == '|')
			special_char(&head, ctx);
		else
			save_not_pipe(&head, ctx);
	}
	if (ctx->cmd_str)
	{
		add_node(&head, new_node(CMD, ctx->cmd_str));
		free(ctx->cmd_str);
	}
	free(ctx);
	return (head);
}
