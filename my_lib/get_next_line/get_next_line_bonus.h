/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdell-er <sdell-er@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 16:55:40 by sdell-er          #+#    #+#             */
/*   Updated: 2024/01/17 16:50:18 by sdell-er         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_BONUS_H
# define GET_NEXT_LINE_BONUS_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

# include <unistd.h>
# include <stdlib.h>

int		ft_strlen2(char *s);

char	*get_next_line(int fd);
char	*ft_calloc2(int nmemb, int size);
char	*ft_strjoin2(char **s1, char *s2);
char	*ft_strchr2(char *s, int c);

void	ft_bzero2(void *s, int n);

#endif