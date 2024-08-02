/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdell-er <sdell-er@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 11:32:06 by sdell-er          #+#    #+#             */
/*   Updated: 2023/10/13 11:32:06 by sdell-er         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char	*str;

	str = (unsigned char *)s;
	while (n--)
	{
		if (*str++ == (unsigned char)c)
			return (--str);
	}
	return (NULL);
}

// void	*ft_memchr(const void *s, int c, size_t n)
// {
// 	int		i;
// 	char	*temp = (char *)s;

// 	i = 0;
// 	while (temp[i] != c && i < n)
// 		i++;
// 	if (n == 0)
// 		return (NULL);
// 	return (s + n - 1 - i);
// }