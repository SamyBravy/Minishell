/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_help2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdell-er <sdell-er@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 14:31:33 by fgrisost          #+#    #+#             */
/*   Updated: 2024/08/30 15:43:16 by sdell-er         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_char(t_expansion_vars_b *vars, t_input *current)
{
	if (vars->current_copy->str[vars->i] == '\'' && vars->in_double_quotes
		== -1)
		single_quotes(vars);
	else if (vars->current_copy->str[vars->i] == '"'
		&& vars->in_single_quotes == -1)
		double_quotes(vars);
	else if (vars->current_copy->str[vars->i] == '$'
		&& vars->in_single_quotes == -1 && vars->current_copy->type != HEREDOC)
		expansion(vars, current);
	else
		vars->result_len++;
}

static t_expansion_vars_b	*init_expansion(t_input *current)
{
	t_expansion_vars_b	*vars;

	vars = malloc(sizeof(t_expansion_vars_b));
	vars->current_copy = current;
	vars->len = ft_strlen(current->str);
	vars->result_len = 0;
	vars->in_single_quotes = -1;
	vars->in_double_quotes = -1;
	vars->i = 0;
	return (vars);
}

size_t	calculate_expanded_length(t_input *current, t_list *env)
{
	t_expansion_vars_b	*vars;
	size_t				result_len;

	vars = init_expansion(current);
	vars->env = env;
	while (vars->i < vars->len)
	{
		check_char(vars, current);
		vars->i++;
	}
	result_len = vars->result_len;
	free(vars);
	return (result_len);
}

static void	append_value(t_expand_vars *vars, char *result)
{
	vars->value_len = ft_strlen(vars->expanded_value);
	vars->j = 0;
	while (vars->j < vars->value_len)
		result[vars->result_index++] = vars->expanded_value[vars->j++];
}

void	expand_variable(t_expand_vars *vars, t_input *current, char *result)
{
	vars->var_name = ft_strndup(current->str + vars->var_start, vars->var_len);
	vars->var_value = ft_getenv(vars->var_name, vars->env);
	if (current->type != CMD && vars->var_value == NULL)
		current->fd = -42;
	free(vars->var_name);
	if (vars->var_value)
	{
		if (vars->in_double_quotes == 1)
			vars->expanded_value = ft_strdup(vars->var_value);
		else
			vars->expanded_value = clean_variable_value(vars->var_value,
					current);
		if (vars->expanded_value)
		{
			append_value(vars, result);
			free(vars->expanded_value);
		}
	}
	vars->i += vars->var_len;
}
