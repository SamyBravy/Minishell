/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 09:31:13 by marvin            #+#    #+#             */
/*   Updated: 2024/08/14 09:31:13 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_number(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (!ft_isdigit(str[i])
			&& !((str[i] == '-' || str[i] == '+') && i == 0))
			return (0);
		i++;
	}
	if (i == 1 && (str[0] == '-' || str[0] == '+'))
		return (0);
	return (1);
}

int	exit_builtin(char **argv, t_input **input)
{
	int	exit_status;

	ft_putstr_fd("exit\n", STDERR_FILENO);
	if (!argv[1])
		exit_status = 0;
	else if (!is_number(argv[1]))
	{
		ft_putstr_fd("minicecco: exit: ", STDERR_FILENO);
		ft_putstr_fd(argv[1], STDERR_FILENO);
		ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
		exit_status = 2;
	}
	else if (argv[2])
		return (ft_putstr_fd("minicecco: exit: too many arguments\n",
				STDERR_FILENO), 1);
	else
	{
		exit_status = ft_atoi(argv[1]);
		if (exit_status < 0)
			exit_status = 256 - ((-exit_status) % 256);
		exit_status %= 256;
	}
	ft_free_mat(argv);
	free_and_exit(input, exit_status);
	return (0);
}
