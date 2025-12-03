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

static void	chenge_quotes_help(t_parse_utils *utils, char *input)
{
	if ((utils->i > 0 && (spaces(input[utils->i - 1]) == 1
				|| input[utils->i - 1] == '>' || input[utils->i - 1] == '<'
				|| input[utils->i - 1] == '|')) && (utils->j < utils->len
			&& (spaces(input[utils->j]) == 1 || input[utils->j] == '>'
				|| input[utils->j] == '<' || input[utils->j] == '|')))
		utils->result[utils->ri++] = '\x1E';
	else if ((utils->i == 0 && (spaces(input[utils->j]) == 1
				|| input[utils->j] == '>' || input[utils->j] == '<'
				|| input[utils->j] == '|')) || (utils->i == 0 && utils->j
			== utils->len) || (utils->j == utils->len
			&& (spaces(input[utils->i - 1]) == 1 || input[utils->i - 1] == '>'
				|| input[utils->i - 1] == '<' || input[utils->i - 1] == '|')))
		utils->result[utils->ri++] = '\x1E';
}

static void	change_quotes(t_parse_utils *utils, char *input)
{
	utils->j = utils->i;
	while ((input[utils->j] == '\"' && utils->j + 1 < utils->len
			&& input[utils->j + 1] == '\"')
		|| (input[utils->j] == '\'' && utils->j + 1 < utils->len
			&& input[utils->j + 1] == '\''))
		utils->j += 2;
	if ((utils->i > 0 && spaces(input[utils->i - 1]) == 0
			&& input[utils->i - 1] != '>' && input[utils->i - 1] != '<'
			&& input[utils->i - 1] != '|') || ((utils->j < utils->len
				&& spaces(input[utils->j]) == 0)
			&& input[utils->j] != '>' && input[utils->j] != '<'
			&& input[utils->j] != '|'))
		utils->i = utils->j;
	else
	{
		chenge_quotes_help(utils, input);
		utils->i = utils->j;
	}
}

char	*quotes_to_special(char *input)
{
	t_parse_utils	utils;

	utils.ri = 0;
	utils.i = 0;
	utils.len = ft_strlen(input);
	utils.result = (char *)malloc(utils.len + 1);
	while (utils.i < utils.len)
	{
		if ((input[utils.i] == '\"' && utils.i + 1 < utils.len && input[utils.i
					+ 1] == '\"') || (input[utils.i] == '\'' && utils.i
				+ 1 < utils.len && input[utils.i + 1] == '\''))
		{
			if (is_inside_quotes(input, utils.i) != 0)
			{
				utils.result[utils.ri++] = input[utils.i++];
				continue ;
			}
			change_quotes(&utils, input);
		}
		else
			utils.result[utils.ri++] = input[utils.i++];
	}
	utils.result[utils.ri] = '\0';
	free(input);
	return (utils.result);
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
