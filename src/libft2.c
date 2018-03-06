/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhonore <qhonore@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/28 16:48:20 by qhonore           #+#    #+#             */
/*   Updated: 2018/03/06 20:24:11 by qhonore          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

int			ft_toupper(int c)
{
	if (c >= 'a' && c <= 'z')
		c -= 32;
	return (c);
}

uint32_t	swap_bin32(uint32_t n, unsigned int magic_number)
{
	if (magic_number == FAT_MAGIC || magic_number == MH_MAGIC\
	|| magic_number == MH_MAGIC_64)
		return (n);
	return (((n >> 24) & 0x0000ff) | ((n << 8) & 0xff0000)\
	| ((n >> 8) & 0x00ff00) | ((n << 24) & 0xff000000));
}

uint64_t	swap_bin64(uint64_t n, unsigned int magic_number)
{
	if (magic_number == FAT_MAGIC || magic_number == MH_MAGIC\
	|| magic_number == MH_MAGIC_64)
		return (n);
	n = (n & 0x00000000FFFFFFFF) << 32 | (n & 0xFFFFFFFF00000000) >> 32;
	n = (n & 0x0000FFFF0000FFFF) << 16 | (n & 0xFFFF0000FFFF0000) >> 16;
	n = (n & 0x00FF00FF00FF00FF) << 8  | (n & 0xFF00FF00FF00FF00) >> 8;
	return (n);
}
