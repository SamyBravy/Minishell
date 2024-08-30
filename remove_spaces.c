/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_spaces.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgrisost <fgrisost@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 14:37:31 by fgrisost          #+#    #+#             */
/*   Updated: 2024/08/30 14:59:19 by fgrisost         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	expand_tokens(t_main_vars *vars, t_list *lst_env)
{
	vars->current = vars->tokens;
	while (vars->current)
	{
		if (vars->current->type == HEREDOC)
			heredoc_quotes(vars->current);
		if (vars->current->type != PIPE)
		{
			vars->expanded_str = expand_variables(vars->current, lst_env);
			if (vars->expanded_str)
			{
				if (vars->current->fd != -42)
				{
					free(vars->current->str);
					vars->current->str = vars->expanded_str;
					vars->expanded_str = NULL;
				}
				else
					free(vars->expanded_str);
			}
		}
		vars->current = vars->current->next;
	}
}

void	free_tokens_and_input(t_main_vars *vars)
{
	while (vars->tokens)
	{
		vars->free_tmp = vars->tokens;
		vars->tokens = vars->tokens->next;
		if (vars->free_tmp->str)
			free(vars->free_tmp->str);
		free(vars->free_tmp);
	}
	free(vars->input);
}

int	just_spaces(const char *str)
{
	if (str == NULL || *str == '\0')
		return (0);
	while (*str != '\0')
	{
		if (*str != '\x1d')
			return (0);
		str++;
	}
	return (1);
}

char	*remove_spaces(char *str)
{
	char	*start;
	char	*str_no_spaces;

	start = str;
	while (*start == '\x1d')
		start++;
	str_no_spaces = calloc(ft_strlen(start) + 1, sizeof(char));
	ft_strlcpy(str_no_spaces, start, INT_MAX);
	free(str);
	return (str_no_spaces);
}