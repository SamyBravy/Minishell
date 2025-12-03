/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   balanced.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samy_bravy <samy_bravy@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 14:28:07 by fgrisost          #+#    #+#             */
/*   Updated: 2024/08/31 10:33:11 by samy_bravy       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_quote_balanced(const char *str, char *quotes)
{
	int	in_double_quote;
	int	in_single_quote;

	in_double_quote = -1;
	in_single_quote = -1;
	while (*str)
	{
		if (*str == '"')
		{
			if (in_single_quote == -1)
				in_double_quote = -1 * in_double_quote;
		}
		else if (*str == '\'')
		{
			if (in_double_quote == -1)
				in_single_quote = -1 * in_single_quote;
		}
		str++;
	}
	*quotes = '"' * (in_double_quote == 1) + '\'' * (in_single_quote == 1);
	if (in_double_quote == -1 && in_single_quote == -1)
		return (1);
	return (-1);
}

int	is_pipe_balanced(char *str)
{
	char	*ptr;

	ptr = str;
	while (*ptr)
	{
		if (*ptr == '|')
		{
			ptr++;
			while (*ptr && spaces(*ptr) == 1)
				ptr++;
			if (*ptr == '\0')
				return (-1);
		}
		ptr++;
	}
	return (1);
}

static void	skip_spaces(char *var_value, t_parse_utils *utils)
{
	while (utils->i < utils->len && spaces(var_value[utils->i]) == 1)
		utils->i++;
}

static void	spaces_check(char *var_value, t_input *current,
		t_parse_utils *utils,
		char *cleaned_value)
{
	int	j;

	j = 0;
	while (spaces(var_value[utils->i]) == 1)
		utils->i++;
	while (utils->i < utils->len)
	{
		if (spaces(var_value[utils->i]) == 1)
		{
			if (current->type != CMD)
				current->fd = -42;
			cleaned_value[utils->cleaned_len++] = '\x1D';
			skip_spaces(var_value, utils);
		}
		else
		{
			cleaned_value[utils->cleaned_len++] = var_value[utils->i++];
			j = utils->i;
			while (var_value[j] && spaces(var_value[j]) == 1)
				j++;
			if (var_value[j] == '\0')
				break ;
		}
	}
}

char	*clean_variable_value(char *var_value, t_input *current)
{
	t_parse_utils	utils;
	char			*cleaned_value;

	utils.len = ft_strlen(var_value);
	cleaned_value = malloc(utils.len + 1);
	if (!cleaned_value)
		return (NULL);
	utils.cleaned_len = 0;
	utils.i = 0;
	while (utils.i < utils.len && spaces(var_value[utils.i]) == 1)
		utils.i++;
	spaces_check(var_value, current, &utils, cleaned_value);
	if (utils.cleaned_len > 0 && cleaned_value[utils.cleaned_len - 1] == ' ')
		utils.cleaned_len--;
	cleaned_value[utils.cleaned_len] = '\0';
	return (cleaned_value);
}
