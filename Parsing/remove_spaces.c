/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_spaces.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samy_bravy <samy_bravy@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 14:37:31 by fgrisost          #+#    #+#             */
/*   Updated: 2024/08/31 10:06:08 by samy_bravy       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
