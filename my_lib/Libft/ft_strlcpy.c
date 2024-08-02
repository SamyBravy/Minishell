/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdell-er <sdell-er@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 19:08:48 by sdell-er          #+#    #+#             */
/*   Updated: 2024/08/01 20:17:25 by sdell-er         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dest, const char *src, size_t size)
{
	size_t	i;

	i = 0;
	if (size > 0)
	{
		while (src[i] && i < (size - 1))
		{
			dest[i] = src[i];
			i += 1;
		}
		dest[i] = '\0';
	}
	while (src[i])
		i += 1;
	return (i);
}

// int main() 
// {
//     char src[] = "coucou";
//     char dest[20];
//     size_t copy;

//     copy = ft_strlcpy(dest, src, sizeof(dest));

//     printf("source : %s\n", src);
//     printf("destination : %s\n", dest);
//     printf("copie : %zu\n", copy);

//     return 0;
// }