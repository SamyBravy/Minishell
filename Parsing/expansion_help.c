/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_help.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samy_bravy <samy_bravy@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 14:29:20 by fgrisost          #+#    #+#             */
/*   Updated: 2024/08/31 10:06:08 by samy_bravy       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	find_value_var(t_expansion_vars_b *vars, t_input *current)
{
	vars->var_name = ft_strndup(current->str + vars->var_start, vars->var_len);
	vars->var_value = ft_getenv(vars->var_name, vars->env);
	free(vars->var_name);
	if (vars->var_value)
	{
		if (vars->in_double_quotes == 1)
			vars->result_len += ft_strlen(vars->var_value);
		else
		{
			vars->cleaned_value = clean_variable_value(vars->var_value,
					vars->current_copy);
			if (vars->cleaned_value)
			{
				vars->result_len += ft_strlen(vars->cleaned_value);
				free(vars->cleaned_value);
			}
		}
	}
	vars->i += vars->var_len;
}

void	expansion_help(t_expansion_vars_b *vars)
{
	while (vars->var_start + vars->var_len < vars->len
		&& ((vars->current_copy->str[vars->var_start + vars->var_len] != '>'
				&& vars->current_copy->str[vars->var_start
					+ vars->var_len] != '<'
				&& vars->current_copy->str[vars->var_start
					+ vars->var_len] != '|'
				&& vars->current_copy->str[vars->var_start
					+ vars->var_len] != '"'
				&& vars->current_copy->str[vars->var_start
					+ vars->var_len] != '\''
				&& vars->current_copy->str[vars->var_start
					+ vars->var_len] != '\x1d'
				&& vars->current_copy->str[vars->var_start
					+ vars->var_len] != '$'
				&& vars->current_copy->str[vars->var_start
					+ vars->var_len] != ' '
				&& vars->current_copy->str[vars->var_start
					+ vars->var_len] != '/')))
	{
		vars->var_len++;
		if (vars->current_copy->str[vars->var_start] == '?')
			break ;
	}
}

void	expansion(t_expansion_vars_b *vars, t_input *current)
{
	vars->var_start = vars->i + 1;
	vars->var_len = 0;
	expansion_help(vars);
	if (vars->var_len > 0)
		find_value_var(vars, current);
	else
		vars->result_len++;
}

void	double_quotes(t_expansion_vars_b *vars)
{
	if (vars->in_single_quotes == -1)
		vars->in_double_quotes *= -1;
	else
		vars->result_len++;
}

void	single_quotes(t_expansion_vars_b *vars)
{
	if (vars->in_double_quotes == -1)
		vars->in_single_quotes *= -1;
	else
		vars->result_len++;
}
