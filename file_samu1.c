/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_samu1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdell-er <sdell-er@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 14:23:05 by fgrisost          #+#    #+#             */
/*   Updated: 2024/08/30 15:49:06 by sdell-er         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	dollar_check(t_vars_samu *vars, char *str)
{
	while (vars->var_start + vars->var_len < vars->len && (str[vars->var_start
				+ vars->var_len] != '>' && str[vars->var_start
				+ vars->var_len] != '<' && str[vars->var_start
				+ vars->var_len] != '|' && str[vars->var_start
				+ vars->var_len] != '"' && str[vars->var_start
				+ vars->var_len] != '\'' && str[vars->var_start
				+ vars->var_len] != '\n' && str[vars->var_start
				+ vars->var_len] != ' ' && str[vars->var_start
				+ vars->var_len] != '$' && str[vars->var_start
				+ vars->var_len] != '/'))
	{
		vars->var_len++;
		if (str[vars->var_start] == '?')
			break ;
	}
	if (vars->var_len > 0)
	{
		vars->var_name = ft_strndup(str + vars->var_start, vars->var_len);
		dollar_check_help_samu(vars);
	}
	else
		vars->result_len++;
}

size_t	calculate_length_samu(char *str, t_list *env)
{
	t_vars_samu	vars;

	vars.i = 0;
	vars.len = ft_strlen(str);
	vars.result_len = 0;
	vars.env = env;
	while (vars.i < vars.len)
	{
		if (str[vars.i] == '$')
		{
			vars.var_start = vars.i + 1;
			vars.var_len = 0;
			dollar_check(&vars, str);
		}
		else
			vars.result_len++;
		vars.i++;
	}
	return (vars.result_len);
}

static void	copy_value(t_vars_samu *vars, char *result)
{
	size_t	value_len;
	size_t	i;

	value_len = ft_strlen(vars->var_value);
	i = 0;
	while (i < value_len)
		result[vars->result_len++] = vars->var_value[i++];
}

void	expand(t_vars_samu *vars, char *str, char *result)
{
	vars->var_name = ft_strndup(str + vars->var_start, vars->var_len);
	vars->var_value = ft_getenv(vars->var_name, vars->env);
	free(vars->var_name);
	if (vars->var_value)
		copy_value(vars, result);
}

void	check_dollar_help(t_vars_samu *vars, char *str, char *result)
{
	while (vars->var_start + vars->var_len < vars->len && (str[vars->var_start
				+ vars->var_len] != '>' && str[vars->var_start
				+ vars->var_len] != '<' && str[vars->var_start
				+ vars->var_len] != '|' && str[vars->var_start
				+ vars->var_len] != '"' && str[vars->var_start
				+ vars->var_len] != '\'' && str[vars->var_start
				+ vars->var_len] != '\n' && str[vars->var_start
				+ vars->var_len] != ' ' && str[vars->var_start
				+ vars->var_len] != '$' && str[vars->var_start
				+ vars->var_len] != '/'))
	{
		vars->var_len++;
		if (str[vars->var_start] == '?')
			break ;
	}
	if (vars->var_len > 0)
	{
		expand(vars, str, result);
		vars->i += vars->var_len;
	}
	else
		result[vars->result_len++] = str[vars->i];
}
