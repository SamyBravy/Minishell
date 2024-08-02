/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdell-er <sdell-er@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/10 18:53:36 by sdell-er          #+#    #+#             */
/*   Updated: 2024/08/01 20:15:24 by sdell-er         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi(const char *nptr)
{
	int	i;
	int	n;
	int	res;

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
// int main() {
//   char  *nptr;
//   nptr = " ---+--+1234ab567";
//   printf("%d", ft_atoi(nptr));
//   return 0;
// }

// int	ft_atoi(const char *str)
// {
// 	int	i;
// 	int	segno;
// 	int	ris;

// 	i = 0;
// 	segno = 1;
// 	ris = 0;
// 	while ((str[i] >= 9 && str[i] <= 13) || str[i] == ' ')
// 		i++;
// 	if (str[i] == '-' || str[i] == '+')
// 	{
// 		if (str[i] == '-')
// 			segno *= -1;
// 		i++;
// 	}
// 	while (str[i] >= '0' && str[i] <= '9')
// 	{
// 		ris *= 10;
// 		ris += str[i] - '0';
// 		i++;
// 	}
// 	return (ris *= segno);
// }
// /*
// int main()
// {
// 	char str[] = "-7364";
// 	printf("%d", ft_atoi(str));
// 	return 0;
// }
// */
