/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgrisost <fgrisost@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 14:40:03 by fgrisost          #+#    #+#             */
/*   Updated: 2024/08/30 14:40:11 by fgrisost         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	tokenize_and_ex(t_main_vars *vars, int *exit_status, t_list **lst_env)
{
	vars->tmp2_str = ft_itoa(*exit_status);
	vars->tmp_str = ft_strjoin("?=", vars->tmp2_str);
	free(vars->tmp2_str);
	ft_export(vars->tmp_str, lst_env, 1);
	free(vars->tmp_str);
	vars->input = quotes_to_special(vars->input);
	vars->tokens = tokenize(vars->input);
	expand_tokens(vars, *lst_env);
	remove_empty_nodes(&vars->tokens);
	free(vars->input);
	executer(&vars->tokens, lst_env, exit_status);
}

int	read_and_signal(t_main_vars *vars, int *exit_status)
{
	g_signal = 0;
	signal(SIGINT, handel_sig_def);
	signal(SIGQUIT, handel_sig_def);
	vars->input = readline("minicecco> ");
	if (g_signal == SIGINT)
	{
		g_signal = 0;
		*exit_status = 130;
		free(vars->input);
		return (1);
	}
	if (!vars->input)
	{
		ft_putstr_fd("exit\n", STDERR_FILENO);
		return (2);
	}
	if (vars->input[0] == '\0')
	{
		free(vars->input);
		return (1);
	}
	return (0);
}

int	syntax_and_quotes(t_main_vars *vars, int *exit_status, char *quotes)
{
	if (check_syntax_errors(vars->input) != 0)
	{
		update_history(vars);
		*exit_status = 2;
		free(vars->input);
		return (1);
	}
	while (is_quote_balanced(vars->input, quotes) == -1)
	{
		if (!ask_more_for_quotes(vars, *quotes))
		{
			if (g_signal == SIGINT)
			{
				g_signal = 0;
				*exit_status = 130;
			}
			else
				*exit_status = 2;
			break ;
		}
	}
	if (!vars->input)
		return (1);
	return (0);
}

int	balanced_pipes_help(t_main_vars *vars, int *exit_status)
{
	if (!ask_more_for_pipes(vars))
	{
		if (g_signal == SIGINT)
		{
			g_signal = 0;
			*exit_status = 130;
		}
		else
		{
			ft_putstr_fd("exit\n", STDERR_FILENO);
			*exit_status = 2;
		}
		return (2);
	}
	else if (!vars->input)
	{
		if (g_signal == SIGINT)
			*exit_status = 130;
		else
			*exit_status = -42;
		g_signal = 0;
		return (2);
	}
	return (0);
}

void	main_loop(t_main_vars *vars, int *exit_status, t_list **lst_env,
		char *quotes)
{
	while (1)
	{
		vars->ret = read_and_signal(vars, exit_status);
		if (vars->ret == 1)
			continue ;
		else if (vars->ret == 2)
			break ;
		if (syntax_and_quotes(vars, exit_status, quotes))
			continue ;
		while (is_pipe_balanced(vars->input) == -1)
		{
			if (balanced_pipes_help(vars, exit_status) == 2)
				break ;
		}
		vars->ret = update_history_and_syntax(vars, exit_status, lst_env);
		if (vars->ret == 1)
			continue ;
		else if (vars->ret == 2)
			break ;
	}
}