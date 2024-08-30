/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_samu2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgrisost <fgrisost@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 14:23:42 by fgrisost          #+#    #+#             */
/*   Updated: 2024/08/30 14:24:06 by fgrisost         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strndup(char *s, size_t n)
{
	char	*src;
	char	*copy;
	size_t	i;

	src = s;
	i = 0;
	if (n < ft_strlen(s))
		copy = malloc(sizeof(char) * (n + 1));
	else
		copy = malloc(sizeof(char) * (ft_strlen(s) + 1));
	while (src[i] && i < n)
	{
		copy[i] = src[i];
		i++;
	}
	copy[i] = '\0';
	return (copy);
}

void	check_dollar(t_vars_samu *vars, char *str, char *result)
{
	if (str[vars->i] == '$')
	{
		vars->var_start = vars->i + 1;
		vars->var_len = 0;
		check_dollar_help(vars, str, result);
	}
	else
		result[vars->result_len++] = str[vars->i];
}

t_vars_samu	*init_expand_samu(const char *str)
{
	t_vars_samu	*vars;

	vars = malloc(sizeof(t_vars_samu));
	vars->i = 0;
	vars->len = ft_strlen(str);
	vars->result_len = 0;
	return (vars);
}

char	*expand_samu(char *str, t_list *env)
{
	t_vars_samu	*vars;
	size_t		expanded_length;
	char		*result;

	vars = init_expand_samu(str);
	vars->env = env;
	expanded_length = calculate_length_samu(str, env);
	result = malloc(expanded_length + 1);
	while (vars->i < vars->len)
	{
		check_dollar(vars, str, result);
		vars->i++;
	}
	result[vars->result_len] = '\0';
	free(vars);
	return (result);
}