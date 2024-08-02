/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdell-er <sdell-er@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 19:05:51 by sdell-er          #+#    #+#             */
/*   Updated: 2024/08/01 20:14:12 by sdell-er         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	sl;
	size_t	dl;

	if (!dst && !src)
		return (0);
	sl = ft_strlen(src);
	if (size == 0)
		return (sl);
	dl = 0;
	while (dst[dl])
		dl += 1;
	i = 0;
	while (src[i] && dl + i < (size - 1))
	{
		dst[i + dl] = src[i];
		i += 1;
	}
	if (i < size)
		dst[dl + i] = '\0';
	if (dl > size)
		return (sl + size);
	return (dl + sl);
}
// int main()
// {
//     char dst[100] = "c'est moi";
//     char src[] = "";
//     printf("%ld", ft_strlcat(dst, src, 15));
//     return 0;
// }
// size_t	ft_strlcat(char *dst, const char *src, size_t size)
// {
// 	unsigned int	i;
// 	unsigned int	r;
// 	unsigned int	destlen;
// 	unsigned int	srclen;

// 	i = 0;
// 	r = 0;
// 	while (dst[r] != '\0')
// 		r++;
// 	destlen = r;
// 	srclen = ft_strlen(src);
// 	if (size == 0 || size <= destlen)
// 		return (srclen + size);
// 	while (src[i] != '\0' && i < size - destlen - 1)
// 	{
// 		dst[r] = src[i];
// 		i++;
// 		r++;
// 	}
// 	dst[r] = '\0';
// 	return (destlen + srclen);
// }
