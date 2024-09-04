/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdell-er <sdell-er@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 14:40:03 by fgrisost          #+#    #+#             */
/*   Updated: 2024/09/04 12:43:41 by sdell-er         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

static int	read_and_signal(t_main_vars *vars, int *exit_status)
{
	char	cwd[PATH_MAX];
	char	*tmp;
	char	*tmp2;

	signal(SIGINT, handel_sig_def);
	signal(SIGQUIT, handel_sig_def);
	getcwd(cwd, sizeof(cwd));
	tmp = ft_strjoin("\033[0;32m", cwd);
	tmp2 = ft_strjoin(tmp, "\033[0m> ");
	free(tmp);
	tmp = ft_strjoin(PROMPT, tmp2);
	free(tmp2);
	vars->input = readline(tmp);
	if (free(tmp), g_signal == SIGINT)
	{
		g_signal = 0;
		*exit_status = 130;
		return (free(vars->input), 1);
	}
	if (!vars->input)
		return (ft_putstr_fd("exit\n", STDERR_FILENO), 2);
	if (vars->input[0] == '\0')
		return (free(vars->input), 1);
	return (0);
}

static int	syntax_and_quotes(t_main_vars *vars, int *exit_status)
{
	char	quotes;

	if (check_syntax_errors(vars->input) != 0)
	{
		update_history(vars);
		*exit_status = 2;
		free(vars->input);
		return (1);
	}
	while (is_quote_balanced(vars->input, &quotes) == -1)
	{
		if (!ask_more_for_quotes(vars, quotes))
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
	return (!vars->input);
}

static int	balanced_pipes_help(t_main_vars *vars, int *exit_status)
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

void	main_loop(t_main_vars *vars, int *exit_status, t_list **lst_env)
{
	while (1)
	{
		g_signal = 0;
		vars->ret = read_and_signal(vars, exit_status);
		if (vars->ret == 1)
			continue ;
		else if (vars->ret == 2)
			break ;
		if (syntax_and_quotes(vars, exit_status))
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
