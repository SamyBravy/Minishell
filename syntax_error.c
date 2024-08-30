/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdell-er <sdell-er@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 11:43:29 by sdell-er          #+#    #+#             */
/*   Updated: 2024/08/30 11:43:31 by sdell-er         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	syntax_error(char *error_token)
{
	ft_putstr_fd("minicecco: syntax error near unexpected token `",
		STDERR_FILENO);
	ft_putstr_fd(error_token, STDERR_FILENO);
	ft_putstr_fd("'\n", STDERR_FILENO);
	return (-1);
}

int	jump_space(char **str)
{
	char	*temp_str;

	temp_str = *str;
	while (**str == ' ')
		(*str)++;
	return (*str != temp_str);
}

int	jump_quotes(char **str)
{
	char	quote;
	char	*temp_str;

	temp_str = *str;
	if (**str == '\'' || **str == '\"')
	{
		quote = **str;
		(*str)++;
		while (**str && **str != quote)
			(*str)++;
		if (**str)
			(*str)++;
	}
	return (*str != temp_str);
}

int	control_redirect(char **token)
{
	if (identify_type(*token) == HEREDOC
		|| identify_type(*token) == APPEND
		|| identify_type(*token) == INPUT || identify_type(*token) == TRUNC)
	{
		if (identify_type(*token) == HEREDOC || identify_type(*token) == APPEND)
			*token += 2;
		else
			(*token)++;
		jump_space(token);
		if (identify_type(*token) == PIPE)
			return (syntax_error("|"));
		if (identify_type(*token) == HEREDOC)
			return (syntax_error("<<"));
		if (identify_type(*token) == APPEND)
			return (syntax_error(">>"));
		if (identify_type(*token) == INPUT)
			return (syntax_error("<"));
		if (identify_type(*token) == TRUNC)
			return (syntax_error(">"));
		if (identify_type(*token) == CMD && !**token)
			return (syntax_error("newline"));
		return (0);
	}
	return (0);
}

int	check_syntax_errors(char *original_token)
{
	char	*token;

	token = original_token;
	jump_space(&token);
	if (identify_type(token) == PIPE)
		return (syntax_error("|"));
	while (*token)
	{
		while (jump_space(&token) || jump_quotes(&token))
			;
		if (!*token)
			break ;
		if (identify_type(token) == PIPE)
		{
			token++;
			jump_space(&token);
			if (identify_type(token) == PIPE)
				return (syntax_error("|"));
		}
		else if (control_redirect(&token))
			return (-1);
		else
			token++;
	}
	return (0);
}
