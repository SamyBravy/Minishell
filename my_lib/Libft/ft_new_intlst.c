/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_new_intlst.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdell-er <sdell-er@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 11:33:06 by sdell-er          #+#    #+#             */
/*   Updated: 2024/08/15 11:32:06 by sdell-er         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_int_list	*ft_new_intlst(int value)
{
	t_int_list	*new;

	new = (t_int_list *)malloc(sizeof(t_int_list));
	if (!new)
		return (NULL);
	new->content = value;
	new->next = NULL;
	return (new);
}
