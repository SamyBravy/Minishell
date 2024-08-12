/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdell-er <sdell-er@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 13:12:17 by marvin            #+#    #+#             */
/*   Updated: 2023/11/06 15:01:15 by sdell-er         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_putnbr(long n)
{
	int	dim;

	dim = 0;
	if (n < 0)
	{
		dim += ft_putchar('-');
		return (dim + ft_putnbr(-n));
	}
	if (n > 9)
		dim += ft_putnbr(n / 10);
	dim += ft_putchar(n % 10 + '0');
	return (dim);
}
