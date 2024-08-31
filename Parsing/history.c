/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samy_bravy <samy_bravy@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 14:35:30 by fgrisost          #+#    #+#             */
/*   Updated: 2024/08/31 10:12:52 by samy_bravy       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	dollar_check_help_samu(t_vars_samu *vars)
{
	vars->var_value = ft_getenv(vars->var_name, vars->env);
	free(vars->var_name);
	if (vars->var_value)
		vars->result_len += strlen(vars->var_value);
	vars->i += vars->var_len;
}

static void	save_history(int fd, const char *entry)
{
	write(fd, entry, ft_strlen(entry));
	write(fd, "\n", 1);
}

void	update_history(t_main_vars *vars)
{
	if (*vars->input)
	{
		vars->history_fd = open(".history",
				O_WRONLY | O_APPEND | O_EXCL, 0644);
		add_history(vars->input);
		if (vars->history_fd != -1)
		{
			save_history(vars->history_fd, vars->input);
			close(vars->history_fd);
		}
	}
}

void	take_history(t_main_vars *vars)
{
	vars->history_fd = open(".history", O_RDONLY);
	if (vars->history_fd == -1)
	{
		vars->history_fd = open(".history", O_CREAT | O_WRONLY, 0644);
		close(vars->history_fd);
		vars->history_fd = open(".history", O_RDONLY);
	}
	if (vars->history_fd != -1)
	{
		vars->line = get_next_line(vars->history_fd);
		while (vars->line)
		{
			vars->line[ft_strlen(vars->line) - 1] = '\0';
			add_history(vars->line);
			free(vars->line);
			vars->line = get_next_line(vars->history_fd);
		}
		get_next_line(-vars->history_fd);
		close(vars->history_fd);
	}
}

int	update_history_and_syntax(t_main_vars *vars, int *exit_status,
		t_list **lst_env)
{
	if (!vars->input && *exit_status == 2)
		return (2);
	else if (!vars->input && *exit_status == 130)
		return (1);
	else if (!vars->input)
	{
		*exit_status = 2;
		return (2);
	}
	update_history(vars);
	if (check_syntax_errors(vars->input) != 0)
	{
		*exit_status = 2;
		free(vars->input);
		return (1);
	}
	tokenize_and_ex(vars, exit_status, lst_env);
	return (0);
}
