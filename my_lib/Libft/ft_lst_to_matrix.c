/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lst_to_matrix.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdell-er <sdell-er@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 11:33:06 by sdell-er          #+#    #+#             */
/*   Updated: 2024/08/15 11:32:06 by sdell-er         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	**ft_lst_to_matrix(t_list *lst)
{
	char	**matrix;
	int		i;

	matrix = (char **)malloc(sizeof(char *) * (ft_lstsize(lst) + 1));
	if (!matrix)
		return (NULL);
	i = 0;
	while (lst)
	{
		matrix[i] = ft_strdup(lst->content);
		if (!matrix[i])
		{
			ft_free_mat(matrix);
			return (NULL);
		}
		lst = lst->next;
		i++;
	}
	matrix[i] = NULL;
	return (matrix);
}
