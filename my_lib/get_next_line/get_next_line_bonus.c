/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdell-er <sdell-er@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 16:55:42 by sdell-er          #+#    #+#             */
/*   Updated: 2024/02/02 17:59:56 by sdell-er         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

void	*ft_memcpy2(void *dest, void *src, int n)
{
	int		i;

	if (!dest || !src)
		return (NULL);
	i = 0;
	while (i < n)
	{
		((unsigned char *)dest)[i] = ((unsigned char *)src)[i];
		i++;
	}
	return (dest);
}

char	*ft_substr2(char **s, int start)
{
	int			i;
	int			d;
	char		*new;

	if (start < ft_strlen2(*s))
		d = ft_strlen2(*s) - start;
	else
		d = 0;
	new = malloc(sizeof(char) * (d + 1));
	if (!new)
	{
		free(*s);
		return (NULL);
	}
	i = 0;
	while (i < d)
	{
		new[i] = (*s)[start + i];
		i++;
	}
	new[i] = '\0';
	free(*s);
	return (new);
}

int	get_current_line(int fd, char **store, char **buffer, int len)
{
	while (len == BUFFER_SIZE && ft_strchr2(*store, '\n') == NULL)
	{
		ft_bzero2(*buffer, BUFFER_SIZE + 1);
		len = read(fd, *buffer, BUFFER_SIZE);
		if (len < 0 || (len == 0 && !ft_strlen2(*store)))
		{
			free(*buffer);
			if (*store)
			{
				free(*store);
				*store = NULL;
			}
			return (0);
		}
		else
		{
			*store = ft_strjoin2(store, *buffer);
			if (!*store)
			{
				free(*buffer);
				return (0);
			}
		}
	}
	return (1);
}

int	set_current_line(char **store, char **buffer)
{
	int	len;

	if (ft_strchr2(*store, '\n') != NULL)
		len = ft_strchr2(*store, '\n') - *store + 1;
	else
		len = ft_strlen2(*store);
	*buffer = ft_calloc2(len + 1, sizeof(char));
	if (!*buffer)
		return (0);
	ft_memcpy2(*buffer, *store, len);
	if (ft_strchr2(*store, '\n') != NULL)
	{
		*store = ft_substr2(store, ft_strlen2(*buffer));
		if (!*store)
		{
			free(*buffer);
			return (0);
		}
	}
	else
	{
		free(*store);
		*store = NULL;
	}
	return (1);
}

char	*get_next_line(int fd)
{
	char			*buffer;
	static char		*store[4096];

	if (fd < 0 || BUFFER_SIZE <= 0 || fd >= 4096)
	{
		if (fd < 0 && store[-fd])
		{
			free(store[-fd]);
			store[-fd] = NULL;
		}
		return (NULL);
	}
	buffer = malloc(BUFFER_SIZE + 1);
	if (!buffer
		|| !get_current_line(fd, store + fd, &buffer, BUFFER_SIZE))
		return (NULL);
	free(buffer);
	if (!set_current_line(store + fd, &buffer))
		return (NULL);
	if (*store && !**store)
	{
		free(*store);
		*store = NULL;
	}
	return (buffer);
}
