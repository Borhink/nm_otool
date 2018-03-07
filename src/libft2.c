/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhonore <qhonore@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/28 16:48:20 by qhonore           #+#    #+#             */
/*   Updated: 2018/03/07 22:58:40 by qhonore          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

int			ft_toupper(int c)
{
	if (c >= 'a' && c <= 'z')
		c -= 32;
	return (c);
}

uint32_t	swap32(uint32_t n, unsigned int magic_number)
{
	if (magic_number == FAT_MAGIC || magic_number == MH_MAGIC\
	|| magic_number == MH_MAGIC_64)
		return (n);
	return (((n >> 24) & 0x0000ff) | ((n << 8) & 0xff0000)\
	| ((n >> 8) & 0x00ff00) | ((n << 24) & 0xff000000));
}

uint64_t	swap64(uint64_t n, unsigned int magic_number)
{
	if (magic_number == FAT_MAGIC || magic_number == MH_MAGIC\
	|| magic_number == MH_MAGIC_64)
		return (n);
	n = (n & 0x00000000FFFFFFFF) << 32 | (n & 0xFFFFFFFF00000000) >> 32;
	n = (n & 0x0000FFFF0000FFFF) << 16 | (n & 0xFFFF0000FFFF0000) >> 16;
	n = (n & 0x00FF00FF00FF00FF) << 8 | (n & 0xFF00FF00FF00FF00) >> 8;
	return (n);
}

void	print_filename(t_env *e)
{
	if (e->mult || e->mult_arch)
	{
		if (!e->mult_arch || (e->mult_arch && e->nfat_arch > 1))
			ft_putchar('\n');
		ft_putstr(e->filename);
		if (e->mult_arch && e->nfat_arch > 1)
		{
			ft_putstr(" (for architecture ");
			if (swap32(e->header->cputype, e->magic) == CPU_TYPE_POWERPC
			|| swap32(e->header->cputype, e->magic) == CPU_TYPE_POWERPC64)
				ft_putstr("ppc");
			else if (swap32(e->header->cputype, e->magic) == CPU_TYPE_I386)
				ft_putstr("i386");
			ft_putchar(')');
		}
		ft_putstr(":\n");
	}
}

int		ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t		i;

	i = 0;
	if (n == 0)
		return (0);
	while (s1[i] != '\0' && s2[i] != '\0' && i < n)
	{
		if (s1[i] != s2[i])
			return (((unsigned char*)s1)[i] - ((unsigned char*)s2)[i]);
		i++;
	}
	if (i == n)
		i--;
	return (((unsigned char*)s1)[i] - ((unsigned char*)s2)[i]);
}
