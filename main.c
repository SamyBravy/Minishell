/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samy_bravy <samy_bravy@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 13:47:54 by fgrisost          #+#    #+#             */
/*   Updated: 2024/08/31 10:28:47 by samy_bravy       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **env)
{
	t_main_vars	vars;
	t_list		*lst_env;
	int			exit_status;

	g_signal = 0;
	exit_status = 0;
	(void)argc;
	(void)argv;
	take_history(&vars);
	lst_env = ft_matrix_to_lst(env);
	if (ft_getenv("OLDPWD", lst_env) == NULL)
		ft_export("OLDPWD", &lst_env, 0);
	vars.tmp = dup(STDIN_FILENO);
	dup2(vars.tmp, STDIN_FILENO);
	close(vars.tmp);
	vars.tmp = dup(STDOUT_FILENO);
	dup2(vars.tmp, STDOUT_FILENO);
	close(vars.tmp);
	main_loop(&vars, &exit_status, &lst_env);
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	ft_lstclear(&lst_env, free);
	return (exit_status);
}
