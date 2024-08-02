/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putexa.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdell-er <sdell-er@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 13:34:30 by marvin            #+#    #+#             */
/*   Updated: 2023/11/06 14:58:44 by sdell-er         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_putexa(unsigned long n, int upper)
{
	int		dim;
	char	*base;

	dim = 0;
	base = "0123456789abcdef";
	if (upper)
		base = "0123456789ABCDEF";
	if (n > 15)
		dim += ft_putexa(n / 16, upper);
	dim += ft_putchar(base[n % 16]);
	return (dim);
}
