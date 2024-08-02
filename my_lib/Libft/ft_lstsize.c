/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdell-er <sdell-er@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/15 10:15:21 by sdell-er          #+#    #+#             */
/*   Updated: 2023/10/15 10:15:21 by sdell-er         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_lstsize(t_list *lst)
{
	int		l;
	t_list	*temp;

	l = 0;
	temp = lst;
	while (temp != NULL)
	{
		temp = temp->next;
		l++;
	}
	return (l);
}
