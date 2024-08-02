/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdell-er <sdell-er@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/10 11:50:25 by sdell-er          #+#    #+#             */
/*   Updated: 2024/08/01 20:12:46 by sdell-er         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_bzero(void *s, size_t n)
{
	size_t	i;
	char	*arr;

	arr = (char *) s;
	i = 0;
	while (i < n)
	{
		arr[i] = '\0';
		i++;
	}
}
// int main()
// {
//     char s[10] = "Hello";
//     size_t n = 5;
//     printf("av : %s\n", s);
//     ft_bzero(s, n);
//     printf("ap : %s\n", s);
//     return 0;
// }