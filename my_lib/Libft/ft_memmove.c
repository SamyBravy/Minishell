/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdell-er <sdell-er@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 11:32:14 by sdell-er          #+#    #+#             */
/*   Updated: 2023/10/13 11:32:14 by sdell-er         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	char	*dest;
	char	*sourc;

	if (!dst && !src)
		return (NULL);
	dest = (char *)dst;
	sourc = (char *)src;
	if (dst == src)
		return (dst);
	if (sourc < dest)
	{
		while (len--)
			*(dest + len) = *(sourc + len);
		return (dst);
	}
	while (len--)
		*dest++ = *sourc++;
	return (dst);
}
// int main()
// {
//     char s[] = "coucou";
//     char s1[20];
//     size_t len = strlen(s);
//     ft_memmove(s1, s, len);
//     printf("%s", s1);
//     return 0;
// }