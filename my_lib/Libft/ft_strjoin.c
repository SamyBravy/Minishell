/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdell-er <sdell-er@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 11:32:29 by sdell-er          #+#    #+#             */
/*   Updated: 2023/10/13 11:32:29 by sdell-er         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// char	*ft_strcpy(char *dest, char *src)
// {
// 	int	n;

// 	n = 0;
// 	while (src[n] != '\0')
// 	{
// 		dest[n] = src[n];
// 		n += 1;
// 	}
// 	dest[n] = '\0';
// 	return (dest);
// }

// char	*ft_strjoin(char const *s1, char const *s2)
// {
// 	char	*dest;
// 	int		i;
// 	int		k;

// 	i = 0;
// 	k = 0;
// 	while (i < size)
// 		k += ft_strlen(strs[i++]);
// 	if (size > 0)
// 		k += ft_strlen(sep) * size - 1;
// 	dest = malloc (sizeof(char) * k + 1);
// 	if (!(dest))
// 		return (NULL);
// 	i = 0;
// 	k = 0;
// 	while (i < size)
// 	{
// 		ft_strcpy(dest + k, strs[i]);
// 		k += ft_strlen(strs[i]);
// 		if (++i < size)
// 			k += dest + k + ft_strlen(sep) - ft_strcpy(dest + k, sep);
// 	}
// 	dest[k] = '\0';
// 	return (dest);
// }

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t		i;
	char		*new;

	if (!s1 || !s2)
		return (NULL);
	new = malloc (sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (!new)
		return (NULL);
	i = 0;
	while (i < ft_strlen(s1))
	{
		new[i] = s1[i];
		i++;
	}
	i = 0;
	while (i < ft_strlen(s2))
	{
		new[i + ft_strlen(s1)] = s2[i];
		i++;
	}
	new[i + ft_strlen(s1)] = '\0';
	return (new);
}
// #include <stdio.h>
// int main()
// {
//     char const *strs = "Hello";
//     char const *sep = " ciao";

// 	printf("%s\n", ft_strjoin(strs, sep));

//     return 0;
// }