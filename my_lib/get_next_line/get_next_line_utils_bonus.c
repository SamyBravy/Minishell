/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdell-er <sdell-er@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 16:43:25 by sdell-er          #+#    #+#             */
/*   Updated: 2024/01/19 18:13:06 by sdell-er         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

int	ft_strlen2(char *s)
{
	int	l;

	if (!s)
		return (0);
	l = 0;
	while (s[l])
		l++;
	return (l);
}

void	ft_bzero2(void *s, int n)
{
	int		i;
	char	*arr;

	arr = (char *) s;
	i = 0;
	while (i < n)
	{
		arr[i] = '\0';
		i++;
	}
}

char	*ft_calloc2(int nmemb, int size)
{
	void	*ptr;

	if (!nmemb || !size)
		return (NULL);
	ptr = malloc(nmemb * size);
	if (!ptr)
		return (NULL);
	ft_bzero2(ptr, nmemb * size);
	return (ptr);
}

char	*ft_strjoin2(char **s1, char *s2)
{
	int			i;
	int			l2;
	int			l1;
	char		*new;

	l1 = ft_strlen2(*s1);
	l2 = ft_strlen2(s2);
	new = malloc (sizeof(char) * (l1 + l2 + 1));
	if (!new)
	{
		if (*s1 != NULL)
			free(*s1);
		return (NULL);
	}
	i = -1;
	while (++i < l1)
		new[i] = (*s1)[i];
	i = -1;
	while (++i < l2)
		new[i + l1] = s2[i];
	new[i + l1] = '\0';
	if (*s1 != NULL)
		free(*s1);
	return (new);
}

char	*ft_strchr2(char *s, int c)
{
	if (!s)
		return (NULL);
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
