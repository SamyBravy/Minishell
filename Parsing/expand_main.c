/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_main.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samy_bravy <samy_bravy@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 14:33:17 by fgrisost          #+#    #+#             */
/*   Updated: 2024/08/31 10:06:08 by samy_bravy       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	variable_expansion_help(t_expand_vars *vars)
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

void	variable_expansion(t_expand_vars *vars, t_input *current, char *result)
{
	vars->var_start = vars->i + 1;
	vars->var_len = 0;
	variable_expansion_help(vars);
	if (vars->var_len > 0)
		expand_variable(vars, current, result);
	else
		result[vars->result_index++] = current->str[vars->i];
}

static void	check_character(t_expand_vars *vars, t_input *current, char *result)
{
	if (current->str[vars->i] == '\'' && vars->in_double_quotes == -1)
		vars->in_single_quotes *= -1;
	else if (current->str[vars->i] == '"' && vars->in_single_quotes == -1)
		vars->in_double_quotes *= -1;
	else if (current->str[vars->i] == '$' && vars->in_single_quotes != 1
		&& current->type != HEREDOC)
		variable_expansion(vars, current, result);
	else
		result[vars->result_index++] = current->str[vars->i];
}

static t_expand_vars	*init_t_expand_vars(t_input *current, t_list *env)
{
	t_expand_vars	*vars;

	vars = malloc(sizeof(t_expand_vars));
	vars->current_copy = current;
	vars->env = env;
	vars->result_len = calculate_expanded_length(current, env);
	vars->len = ft_strlen(current->str);
	vars->in_single_quotes = -1;
	vars->in_double_quotes = -1;
	vars->i = 0;
	return (vars);
}

char	*expand_variables(t_input *current, t_list *env)
{
	t_expand_vars	*vars;
	char			*result;

	vars = init_t_expand_vars(current, env);
	result = malloc(vars->result_len + 1);
	vars->result_index = 0;
	while (vars->i < vars->len)
	{
		check_character(vars, current, result);
		vars->i++;
	}
	result[vars->result_index] = '\0';
	free(vars);
	return (result);
}
