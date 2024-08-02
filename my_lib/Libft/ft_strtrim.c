/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdell-er <sdell-er@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 11:31:44 by sdell-er          #+#    #+#             */
/*   Updated: 2023/10/13 11:31:44 by sdell-er         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	present(const char *set, char c)
{
	size_t	i;

	i = 0;
	while (i < ft_strlen(set))
	{
		if (set[i] == c)
			return (1);
		i++;
	}
	return (0);
}

static int	new_dim(int fine, char const *set, char const *s1)
{
	int	i;
	int	j;
	int	poss;

	i = -1;
	j = 0;
	poss = 1;
	while (++i < fine)
	{
		if (poss && !present(set, s1[i]))
			poss = 0;
		if (!poss)
			j++;
	}
	return (j);
}

static int	calculate_i(char const *s1, char const *set)
{
	int	i;

	i = ft_strlen(s1) - 1;
	while (i >= 0 && present(set, s1[i]))
		i--;
	return (i);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	int		fine;
	int		i;
	int		j;
	int		poss;
	char	*new;

	if (!s1 || !set)
		return (NULL);
	i = calculate_i(s1, set);
	fine = i + 1;
	i = -1;
	j = 0;
	poss = 1;
	new = malloc(sizeof(char) * (new_dim(fine, set, s1) + 1));
	if (!new)
		return (NULL);
	while (++i < fine)
	{
		if (poss && !present(set, s1[i]))
			poss = 0;
		if (!poss)
			new[j++] = s1[i];
	}
	new[j] = '\0';
	return (new);
}
//   #include<stdio.h>
//   int main()
//   {
//   	char *s1 = "cackncnonna22skxaxca";
//   	char *set = "cank";
//   	printf("%s", ft_strtrim(s1, set));
//   	return 0;
//   }
