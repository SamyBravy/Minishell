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

static int	is_number(char *str)
{
	int	i;

	i = -1;
	while (str[++i])
	{
		if (!ft_isdigit(str[i])
			&& !((str[i] == '-' || str[i] == '+') && i == 0))
			return (0);
	}
	if (i == 1 && (str[0] == '-' || str[0] == '+'))
		return (0);
	if (str[0] == '+')
	{
		if (ft_strcmp(str, "+9223372036854775807") > 0)
			return (0);
	}
	else if (str[0] == '-')
	{
		if (ft_strcmp(str, "-9223372036854775808") > 0)
			return (0);
	}
	else
		if (ft_strcmp(str, "9223372036854775807") > 0)
			return (0);
	return (1);
}

static long long int	ft_long_long_atoi(const char *nptr)
{
	int				i;
	int				n;
	long long int	res;

	n = 0;
	i = 1;
	res = 0;
	while ((nptr[n] >= 9 && nptr[n] <= 13) || nptr[n] == ' ')
		n += 1;
	if (nptr[n] == '-' || nptr[n] == '+')
	{
		if (nptr[n] == '-')
			i *= -1;
		n += 1;
	}
	while (nptr[n] >= '0' && nptr[n] <= '9')
		res = res * 10 + (nptr[n++] - '0');
	return (res * i);
}

int	exit_builtin(char **argv, t_input **input, int *pipefd)
{
	long long int	exit_status;

	if (!pipefd)
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
		return (ft_putstr_fd("minicecco: exit: too many arguments\n", 2), 1);
	else
	{
		exit_status = ft_long_long_atoi(argv[1]);
		if (exit_status < 0)
			exit_status = 256 - ((-exit_status) % 256);
		exit_status %= 256;
	}
	ft_free_mat(argv);
	clean_and_exit(input, exit_status, pipefd);
	return (0);
}
