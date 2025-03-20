/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_and_quotes.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samy_bravy <samy_bravy@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 14:34:09 by fgrisost          #+#    #+#             */
/*   Updated: 2024/08/31 10:06:08 by samy_bravy       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	is_inside_quotes(const char *str, int index)
{
	int	i;
	int	in_double_quotes;
	int	in_single_quotes;

	in_double_quotes = -1;
	in_single_quotes = -1;
	i = 0;
	while (i < index)
	{
		if (str[i] == '\"' && in_single_quotes == -1)
			in_double_quotes = -1 * in_double_quotes;
		else if (str[i] == '\'' && in_double_quotes == -1)
			in_single_quotes = -1 * in_single_quotes;
		i++;
	}
	if (in_double_quotes == 1)
		return (1);
	if (in_single_quotes == 1)
		return (2);
	return (0);
}

static void	chenge_quotes_help(t_quotes_special_vars *vars, char *input)
{
	if ((vars->i > 0 && (spaces(input[vars->i - 1]) == 1
				|| input[vars->i - 1] == '>' || input[vars->i - 1] == '<'
				|| input[vars->i - 1] == '|')) && (vars->j < vars->length
			&& (spaces(input[vars->j]) == 1 || input[vars->j] == '>'
				|| input[vars->j] == '<' || input[vars->j] == '|')))
		vars->result[vars->ri++] = '\x1E';
	else if ((vars->i == 0 && (spaces(input[vars->j]) == 1
				|| input[vars->j] == '>' || input[vars->j] == '<'
				|| input[vars->j] == '|')) || (vars->i == 0 && vars->j
			== vars->length) || (vars->j == vars->length
			&& (spaces(input[vars->i - 1]) == 1 || input[vars->i - 1] == '>'
				|| input[vars->i - 1] == '<' || input[vars->i - 1] == '|')))
		vars->result[vars->ri++] = '\x1E';
}

static void	change_quotes(t_quotes_special_vars *vars, char *input)
{
	vars->j = vars->i;
	while ((input[vars->j] == '\"' && vars->j + 1 < vars->length
			&& input[vars->j + 1] == '\"') || (input[vars->j] == '\'' && vars->j
			+ 1 < vars->length && input[vars->j + 1] == '\''))
		vars->j += 2;
	if ((vars->i > 0 && spaces(input[vars->i - 1]) == 0
			&& input[vars->i - 1] != '>' && input[vars->i - 1] != '<'
			&& input[vars->i - 1] != '|') || ((vars->j < vars->length
				&& spaces(input[vars->j]) == 0)
			&& input[vars->j] != '>' && input[vars->j] != '<'
			&& input[vars->j] != '|'))
		vars->i = vars->j;
	else
	{
		chenge_quotes_help(vars, input);
		vars->i = vars->j;
	}
}

char	*quotes_to_special(char *input)
{
	t_quotes_special_vars	vars;

	vars.ri = 0;
	vars.i = 0;
	vars.length = ft_strlen(input);
	vars.result = (char *)malloc(vars.length + 1);
	while (vars.i < vars.length)
	{
		if ((input[vars.i] == '\"' && vars.i + 1 < vars.length && input[vars.i
					+ 1] == '\"') || (input[vars.i] == '\'' && vars.i
				+ 1 < vars.length && input[vars.i + 1] == '\''))
		{
			if (is_inside_quotes(input, vars.i) != 0)
			{
				vars.result[vars.ri++] = input[vars.i++];
				continue ;
			}
			change_quotes(&vars, input);
		}
		else
			vars.result[vars.ri++] = input[vars.i++];
	}
	vars.result[vars.ri] = '\0';
	free(input);
	return (vars.result);
}

void	heredoc_quotes(t_input *current)
{
	char	*tmp;

	tmp = current->str;
	while (*tmp)
	{
		if (*tmp == '"' || *tmp == '\'')
			current->fd = -420;
		tmp++;
	}
}
