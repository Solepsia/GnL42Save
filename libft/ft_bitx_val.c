/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bitx_val.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grota <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/30 16:21:41 by grota             #+#    #+#             */
/*   Updated: 2017/11/30 16:28:16 by grota            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		ft_bitx_val(int a, int x)
{
	int		b;

	b = 1;
	b = b << x;
	if (a & b)
		return (1);
	else
		return (0);
}
