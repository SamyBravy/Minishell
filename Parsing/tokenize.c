/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samy_bravy <samy_bravy@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 14:26:52 by fgrisost          #+#    #+#             */
/*   Updated: 2024/08/31 10:06:08 by samy_bravy       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	save_cmd(t_token_variables *vars)
{
	char	*help;

	if (vars->cmd_str)
	{
		help = vars->copy_str - ft_strlen(vars->token) - 1;
		vars->temp_cmd_str = calloc(ft_strlen(vars->cmd_str)
				+ ft_strlen(vars->token) + 2, 1);
		ft_strlcpy(vars->temp_cmd_str, vars->cmd_str, INT_MAX);
		if (spaces(*help) == 1)
			ft_strlcat(vars->temp_cmd_str, "\x1D", INT_MAX);
		ft_strlcat(vars->temp_cmd_str, vars->token, INT_MAX);
		free(vars->cmd_str);
		vars->cmd_str = vars->temp_cmd_str;
	}
	else
		vars->cmd_str = ft_strndup(vars->token, INT_MAX);
}

static void	save_special(t_input **head, t_token_variables *vars)
{
	if (vars->cmd_str)
	{
		add_node(head, new_node(CMD, vars->cmd_str));
		free(vars->cmd_str);
		vars->cmd_str = NULL;
	}
	add_node(head, new_node(vars->current_type, vars->token));
	vars->current_type = CMD;
}

static void	save_not_pipe(t_input **head, t_token_variables *vars)
{
	if (*vars->copy_str == '"' || *vars->copy_str == '\'')
		quotes(vars);
	else
		normal_char(vars);
	if (vars->current_type == CMD || vars->current_type == PIPE)
		save_cmd(vars);
	else
		save_special(head, vars);
	free(vars->token);
}

static t_token_variables	*prepare_var(char *str)
{
	t_token_variables	*vars;

	vars = malloc(sizeof(t_token_variables));
	vars->token = NULL;
	vars->current_type = CMD;
	vars->cmd_str = NULL;
	vars->copy_str = str;
	vars->flag = 0;
	return (vars);
}

t_input	*tokenize(char *str)
{
	t_input				*head;
	t_token_variables	*vars;

	head = NULL;
	vars = prepare_var(str);
	while (*vars->copy_str)
	{
		while (spaces(*vars->copy_str) == 1)
			vars->copy_str++;
		if (*vars->copy_str == '\0')
			break ;
		if (*vars->copy_str == '>' || *vars->copy_str == '<'
			|| *vars->copy_str == '|')
			special_char(&head, vars);
		else
			save_not_pipe(&head, vars);
	}
	if (vars->cmd_str)
	{
		add_node(&head, new_node(CMD, vars->cmd_str));
		free(vars->cmd_str);
	}
	free(vars);
	return (head);
}
