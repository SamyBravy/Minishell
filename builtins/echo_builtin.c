/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdell-er <sdell-er@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 11:44:25 by sdell-er          #+#    #+#             */
/*   Updated: 2024/08/30 11:44:25 by sdell-er         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	number_of_flags(char **argv)
{
	int	i;
	int	j;

	i = 1;
	while (argv[i])
	{
		if (argv[i][0] == '-' && argv[i][1] == 'n')
		{
			j = 1;
			while (argv[i][j] == 'n')
				j++;
			if (argv[i][j] == '\0')
				i++;
			else
				break ;
		}
		else
			break ;
	}
	return (i - 1);
}

int	echo_builtin(char **argv)
{
	int	i;

	i = 1 + number_of_flags(argv);
	while (argv[i])
	{
		if (argv[i][0] != '\x1E')
			ft_printf("%s", argv[i]);
		if (argv[i + 1])
			ft_printf(" ");
		i++;
	}
	if (number_of_flags(argv) == 0)
		ft_printf("\n");
	return (0);
}
