/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_matrix_to_lst.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdell-er <sdell-er@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 11:33:06 by sdell-er          #+#    #+#             */
/*   Updated: 2024/08/15 11:32:06 by sdell-er         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_matrix_to_lst(char **matrix)
{
	t_list	*lst;
	int		i;

	if (!matrix)
		return (NULL);
	i = 0;
	lst = ft_lstnew(ft_strdup(matrix[i]));
	if (!lst)
		return (NULL);
	i++;
	while (matrix[i])
	{
		ft_lstadd_back(&lst, ft_lstnew(ft_strdup(matrix[i])));
		if (!lst)
		{
			ft_lstclear(&lst, free);
			return (NULL);
		}
		i++;
	}
	return (lst);
}
