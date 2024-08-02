/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdell-er <sdell-er@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 11:32:34 by sdell-er          #+#    #+#             */
/*   Updated: 2023/10/13 11:32:34 by sdell-er         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t		i;
	size_t		d;
	char		*new;

	if (!s)
		return (NULL);
	if (start + len <= ft_strlen(s))
		d = len;
	else if (start >= ft_strlen(s))
		d = 0;
	else
		d = ft_strlen(s) - start;
	new = malloc(sizeof(char) * (d + 1));
	if (!new)
		return (NULL);
	i = 0;
	while (d && i < len && s[i])
	{
		new[i] = s[start + i];
		i++;
	}
	new[i] = '\0';
	return (new);
}
