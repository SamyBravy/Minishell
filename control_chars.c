/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   control_chars.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdell-er <sdell-er@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 14:25:24 by fgrisost          #+#    #+#             */
/*   Updated: 2024/08/30 15:43:51 by sdell-er         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	spaces(char c)
{
	if ((c == 32 || c == 13 || c == 9 || c == 11))
		return (1);
	return (0);
}

void	special_char(t_input **head, t_token_variables *vars)
{
	vars->i = 0;
	while (vars->i++ <= 2)
		vars->temp[vars->i - 1] = 0;
	vars->temp[0] = *vars->copy_str++;
	if ((*vars->copy_str == '>' || *vars->copy_str == '<')
		&& vars->temp[0] == *vars->copy_str)
		vars->temp[1] = *vars->copy_str++;
	vars->current_type = identify_type(vars->temp);
	if (vars->current_type == PIPE)
	{
		if (vars->cmd_str)
		{
			add_node(head, new_node(CMD, vars->cmd_str));
			free(vars->cmd_str);
			vars->cmd_str = NULL;
		}
		add_node(head, new_node(vars->current_type, NULL));
	}
}

void	quotes(t_token_variables *vars)
{
	vars->c_for_quotes = *vars->copy_str;
	vars->start = vars->copy_str++;
	while (*vars->copy_str && *vars->copy_str != vars->c_for_quotes)
		vars->copy_str++;
	vars->copy_str++;
	vars->token = ft_strndup(vars->start, vars->copy_str - vars->start);
}

void	normal_char(t_token_variables *vars)
{
	vars->start = vars->copy_str;
	while (*vars->copy_str && spaces(*vars->copy_str) == 0
		&& *vars->copy_str != '>' && *vars->copy_str != '<'
		&& *vars->copy_str != '|')
	{
		if (*vars->copy_str == '"' || *vars->copy_str == '\'')
		{
			vars->c_for_quotes = *vars->copy_str;
			vars->copy_str++;
			while (*vars->copy_str && *vars->copy_str != vars->c_for_quotes)
				vars->copy_str++;
		}
		if (*vars->copy_str)
			vars->copy_str++;
	}
	vars->token = ft_strndup(vars->start, vars->copy_str - vars->start);
}
