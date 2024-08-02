/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdell-er <sdell-er@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 11:32:00 by sdell-er          #+#    #+#             */
/*   Updated: 2023/10/13 11:32:00 by sdell-er         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	size_t	slen;

	slen = ft_strlen(s);
	if ((char)c == '\0')
		return ((char *)(s + slen));
	while (slen--)
		if (s[slen] == (char)c)
			return ((char *)(s + slen));
	return (NULL);
}

// char	*ft_strrchr(const char *s, int c)
// {
// 	int	l;

// 	l = ft_strlen(s);
// 	while (l >= 0 && s[l] != c)
// 		l--;
// 	if (l == -1)
// 		return (NULL);
// 	return (s + l);
// }