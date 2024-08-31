/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_builtin.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdell-er <sdell-er@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 11:44:02 by sdell-er          #+#    #+#             */
/*   Updated: 2024/08/30 11:44:02 by sdell-er         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	unset_builtin(char **argv, t_list **env)
{
	int	i;
	int	exit_status;

	exit_status = 0;
	i = 1;
	while (argv[i])
	{
		if (!is_valid_identifier(argv[i], 0))
		{
			ft_putstr_fd("minicecco: unset: `", STDERR_FILENO);
			ft_putstr_fd(argv[i], STDERR_FILENO);
			ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
			exit_status = 1;
		}
		else if (!(argv[i][0] == '_' && argv[i][1] == '\0'))
			lst_remove_key(env, argv[i]);
		i++;
	}
	return (exit_status);
}
