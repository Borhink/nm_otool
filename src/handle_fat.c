/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_fat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhonore <qhonore@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/07 16:09:48 by qhonore           #+#    #+#             */
/*   Updated: 2018/03/07 23:23:05 by qhonore          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

static void	handle_all_arch_32(t_env *e, struct fat_header *header)
{
	uint32_t				i;
	struct fat_arch			*fat;
	struct mach_header_64	*head;

	fat = (void*)header + sizeof(*header);
	i = -1;
	while (++i < swap32(header->nfat_arch, e->fat_magic))
	{
		head = (void*)header + swap32(fat[i].offset, e->fat_magic);
		e->ptr = head;
		e->magic = *((int*)e->ptr);
		if (head->magic == MH_MAGIC_64 || head->magic == MH_CIGAM_64)
			handle_magic_64(e, head);
		else if (head->magic == MH_MAGIC || head->magic == MH_CIGAM)
			handle_magic_32(e, (void*)head);
	}
}

void test(char o)
{
	const char	*base = "0123456789abcdef";

	ft_putchar(base[o / 16]);
	ft_putchar(base[o % 16]);
	ft_putchar(' ');
}

void		handle_fat_32(t_env *e, struct fat_header *header)
{
	uint32_t				i;
	struct fat_arch			*fat;
	struct mach_header_64	*head;

	fat = (void*)header + sizeof(*header);
	i = -1;
	e->nfat_arch = swap32(header->nfat_arch, e->magic);
	while (++i < swap32(header->nfat_arch, e->fat_magic))
	{
		head = (void*)header + swap32(fat[i].offset, e->fat_magic);
		if (!ft_strncmp((void*)head, ARMAG, SARMAG))
			ft_putstr("GET OUT THE WAY, BITCH !!\n");
		if (head->magic == MH_MAGIC_64 || head->magic == MH_CIGAM_64)
		{
			e->ptr = head;
			e->magic = *((int*)e->ptr);
			handle_magic_64(e, head);
			return ;
		}
	}
	e->mult_arch = 1;
	handle_all_arch_32(e, header);
}

static void	handle_all_arch_64(t_env *e, struct fat_header *header)
{
	uint32_t				i;
	struct fat_arch_64		*fat;
	struct mach_header_64	*head;

	fat = (void*)header + sizeof(*header);
	i = -1;
	while (++i < swap32(header->nfat_arch, e->fat_magic))
	{
		head = (void*)header + swap64(fat[i].offset, e->fat_magic);
		e->ptr = head;
		e->magic = *((int*)e->ptr);
		if (head->magic == MH_MAGIC_64 || head->magic == MH_CIGAM_64)
			handle_magic_64(e, head);
		else if (head->magic == MH_MAGIC || head->magic == MH_CIGAM)
			handle_magic_32(e, (void*)head);
	}
}

void		handle_fat_64(t_env *e, struct fat_header *header)
{
	uint32_t				i;
	struct fat_arch_64		*fat64;
	struct mach_header_64	*head;

	fat64 = (void*)header + sizeof(*header);
	i = -1;
	e->nfat_arch = swap32(header->nfat_arch, e->magic);
	while (++i < swap32(header->nfat_arch, e->magic))
	{
		head = (void*)header + swap64(fat64[i].offset, e->magic);
		if (head->magic == MH_MAGIC_64 || head->magic == MH_CIGAM_64)
		{
			e->ptr = head;
			e->magic = *((int*)e->ptr);
			handle_magic_64(e, head);
			return ;
		}
	}
	e->mult_arch = 1;
	handle_all_arch_64(e, header);
}
