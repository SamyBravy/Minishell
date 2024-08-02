/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdell-er <sdell-er@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 11:32:24 by sdell-er          #+#    #+#             */
/*   Updated: 2023/10/13 11:32:24 by sdell-er         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// char	*ft_strchr(const char *s, int c)
// {
// 	char	*str;

// 	str = (char *)s;
// 	while (*s)
// 	{
// 		if (*s == (char)c)
// 			return (str);
// 		s++;
// 	}
// 	if (*s == (char)c)
// 		return (str);
// 	return (NULL);
// }

char	*ft_strchr(const char *s, int c)
{
	while (*s)
	{
		if (*s == (char)c)
			return ((char *)s);
		s++;
	}
	if (*s == (char)c)
		return ((char *)s);
	return (NULL);
}

// char	*ft_strchr(const char *s, int c)
// {
// 	while (*s && *s != c)
// 		s++;
// 	if (*s != c)
// 		return (NULL);
// 	return ((char *)s);
// }