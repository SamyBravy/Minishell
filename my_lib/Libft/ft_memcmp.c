/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdell-er <sdell-er@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 11:31:52 by sdell-er          #+#    #+#             */
/*   Updated: 2023/10/13 11:31:52 by sdell-er         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	const unsigned char	*str1;
	const unsigned char	*str2;
	size_t				i;

	if (!s1 && !s2)
		return (0);
	str1 = (const unsigned char *)s1;
	str2 = (const unsigned char *)s2;
	i = 0;
	while (i < n)
	{
		if (str1[i] != str2[i])
			return (str1[i] - str2[i]);
		i++;
	}
	return (0);
}

// int main()
// {
// 	char *s1 = "mamma mia bella ciao !";
// 	char *s2 = "mammya mia bella ciao !";
// 	printf("%d", ft_memcmp(s1, s2, 8));
// 	return 0;
// }

// int	ft_memcmp(const void *s1, const void *s2, size_t n)
// {
// 	int		i;
// 	char	*temp1;
// 	char	*temp2;

// 	temp1 = (char *)s1;
// 	temp2 = (char *)s2;
// 	i = 0;
// 	while (temp1[i] && temp2[i] && temp1[i] - temp2[i] != 0)
// 		i++;
// 	return (temp1[i] - temp2[i]);
// }