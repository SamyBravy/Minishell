/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   more_pipes_and_quotes.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgrisost <fgrisost@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 14:36:10 by fgrisost          #+#    #+#             */
/*   Updated: 2024/08/30 14:36:33 by fgrisost         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	more_quotes_help(t_main_vars *vars, char quotes)
{
	update_history(vars);
	free(vars->input);
	vars->input = NULL;
	ft_putstr_fd("minicecco: unexpected EOF while looking for matching `",
		STDERR_FILENO);
	ft_putchar_fd(quotes, STDERR_FILENO);
	ft_putstr_fd("'\nminicecco: syntax error: unexpected end of file\n",
		STDERR_FILENO);
	return (false);
}

bool	ask_more_for_quotes(t_main_vars *vars, char quotes)
{
	vars->temp_input = readline("> ");
	if (g_signal == SIGINT)
	{
		update_history(vars);
		free(vars->input);
		vars->input = NULL;
		return (false);
	}
	if (!vars->temp_input)
		return (more_quotes_help(vars, quotes));
	vars->new_input = malloc(ft_strlen(vars->input)
			+ ft_strlen(vars->temp_input) + 2);
	ft_strlcpy(vars->new_input, vars->input, INT_MAX);
	ft_strlcat(vars->new_input, "\n", INT_MAX);
	ft_strlcat(vars->new_input, vars->temp_input, INT_MAX);
	free(vars->input);
	vars->input = vars->new_input;
	free(vars->temp_input);
	return (true);
}

int	more_pipes_help(t_main_vars *vars)
{
	update_history(vars);
	free(vars->input);
	vars->input = NULL;
	ft_putstr_fd("minicecco: syntax error: unexpected end of file\n",
		STDERR_FILENO);
	return (0);
}

int	ask_more_for_pipes(t_main_vars *vars)
{
	char	quotes;

	vars->temp_input = readline("> ");
	if (g_signal == SIGINT)
	{
		update_history(vars);
		free(vars->input);
		vars->input = NULL;
		return (0);
	}
	if (!vars->temp_input)
		return (more_pipes_help(vars));
	vars->new_input = malloc(ft_strlen(vars->input)
			+ ft_strlen(vars->temp_input) + 1);
	ft_strlcpy(vars->new_input, vars->input, INT_MAX);
	ft_strlcat(vars->new_input, vars->temp_input, INT_MAX);
	free(vars->input);
	vars->input = vars->new_input;
	free(vars->temp_input);
	while (is_quote_balanced(vars->input, &quotes) == -1)
	{
		if (!ask_more_for_quotes(vars, quotes))
			return (2);
	}
	return (1);
}