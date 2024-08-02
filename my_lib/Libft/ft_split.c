/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdell-er <sdell-er@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/15 10:16:20 by sdell-er          #+#    #+#             */
/*   Updated: 2024/08/02 11:54:38 by sdell-er         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	if_else_while(struct s_ogg *o)
{
	if (o->str[o->i] != o->charset)
	{
		if (o->poss == 1)
			o->cont++;
		if (o->cont == o->indice + 1 && o->opz == 3)
			o->ottenuto = 1;
		else if (o->cont == o->indice + 1 && o->opz == 2)
			return (o->i);
		o->poss = 0;
	}
	else
	{
		if (o->ottenuto == 1 && o->opz == 3)
			return (o->i - 1);
		o->poss = 1;
	}
	return (-42);
}

static int	opz_parola(char *str, int indice, char charset, int opz)
{
	int				ret;
	struct s_ogg	o;
	struct s_ogg	*ptr;

	ptr = &o;
	o.ottenuto = 0;
	o.poss = 1;
	o.str = str;
	o.indice = indice;
	o.charset = charset;
	o.opz = opz;
	o.i = 0;
	o.cont = 0;
	while (str[o.i])
	{
		ret = if_else_while(ptr);
		if (ret != -42)
			return (ret);
		o.i++;
	}
	if (o.opz == 1)
		return (o.cont);
	return (o.i - 1);
}

static void	riempi(char *dest, char *str, int start, int fine)
{
	int	i;

	i = 0;
	while (start <= fine)
	{
		dest[i] = str[start];
		start++;
		i++;
	}
	dest[i] = '\0';
}

static int	free_mat(char ***mat, int i)
{
	if ((*mat)[i] == NULL)
	{
		free(*mat);
		return (1);
	}
	return (0);
}

char	**ft_split(char const *s, char c)
{
	int		parole;
	int		i;
	char	**mat;
	char	*str;

	if (s == NULL)
		return (NULL);
	str = (char *)s;
	parole = opz_parola(str, 0, c, 1);
	mat = malloc((parole + 1) * sizeof(char *));
	if (mat == NULL)
		return (NULL);
	i = -1;
	while (++i < parole)
	{
		mat[i] = malloc(sizeof(char)
				* (opz_parola(str, i, c, 3)
					- opz_parola(str, i, c, 2) + 2));
		if (free_mat(&mat, i))
			return (NULL);
		riempi(mat[i], str, opz_parola(str, i, c, 2),
			opz_parola(str, i, c, 3));
	}
	mat[i] = NULL;
	return (mat);
}
/*
int main()
{
	char *str = "nonna10nonnavedrainonnonnasononnaquantinonnaanninonna";
	char sep = 'n';
	char **mat = ft_split(str, sep);
	int i = 0;
	while (i < opz_parola(str, 0, sep, 1))
	{
		printf("%s\n", mat[i]);
		i++;
	}
	
	return 0;
}
*/