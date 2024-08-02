/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdell-er <sdell-er@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 08:57:42 by sdell-er          #+#    #+#             */
/*   Updated: 2024/01/17 16:34:16 by sdell-er         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strcmp(char *s1, char *s2)
{
	int	i;
	int	rapp;

	rapp = 0;
	i = 0;
	while (s1[i] != '\0' && s2[i] != '\0' && rapp == 0)
	{
		rapp = s1[i] - s2[i];
		i++;
	}
	if (!(s1[i] == '\0' && s2[i] == '\0') && rapp == 0)
		rapp = s1[i] - s2[i];
	return (rapp);
}
/*
int	main(void)
{
    char s1[] = "Non";
    char s2[] = "Noon";
    
    printf(" %d %d ", ft_strcmp(s1, s2), strcmp(s1, s2));
    return 0;
}
*/
