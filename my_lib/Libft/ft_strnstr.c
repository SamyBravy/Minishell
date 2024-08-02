/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdell-er <sdell-er@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 11:31:33 by sdell-er          #+#    #+#             */
/*   Updated: 2023/10/13 11:31:33 by sdell-er         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	char	*b;
	char	*l;
	size_t	c1;
	size_t	c2;

	if (!big && !little)
		return (NULL);
	b = (char *)big;
	l = (char *)little;
	c1 = 0;
	if (l[c1] == '\0')
		return (b);
	while (b[c1] != '\0')
	{
		c2 = 0;
		while (b[c1 + c2] == l[c2] && c1 + c2 < len)
		{
			if (b[c1 + c2] == '\0' || l[c2++] == '\0')
				break ;
		}
		if (l[c2++] == '\0')
			return (&b[c1]);
		c1++;
	}
	return (0);
}
// char	*ft_strnstr(const char *big, const char *little, size_t len)
// {
// 	char	*b;
// 	size_t	i;
// 	size_t	j;

// 	b = (char *) big;
// 	if (little[0] == '\0')
// 		return (b);
// 	i = 0;
// 	j = 0;
// 	while (i < len && b[i] && little[j] && j != ft_strlen(little))
// 	{
// 		if (b[i] == little[j])
// 			j++;
// 		else if (b[i] == little[0])
// 			j = 1;
// 		else
// 			j = 0;
// 		i++;
// 	}
// 	if (j != ft_strlen(little))
// 		return (NULL);
// 	return (b + i - ft_strlen(little));
// }
// #include <stdio.h>
// int main()
// {
// 	//char *big = "ciao nonna, sono molto nonna contento nonna";
// 	//char *little = "nonna";
// 	printf("->%s", ft_strnstr("lorem ipsum dolor sit amet", "lorem", 15));
// 	return 0;
// }
