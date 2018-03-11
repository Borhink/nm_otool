/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_fat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhonore <qhonore@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/07 16:09:48 by qhonore           #+#    #+#             */
/*   Updated: 2018/03/11 15:06:18 by qhonore          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

static int	handle_all_arch_32(t_env *e, struct fat_header *header)
{
	uint32_t				i;
	struct fat_arch			*fat;
	struct mach_header_64	*head;

	fat = (void*)header + sizeof(*header);
	i = -1;
	while (++i < swap32(header->nfat_arch, e->fat_magic))
	{
		head = (void*)header + swap32(fat[i].offset, e->fat_magic);
		if (corrupted_ptr(e, head))
			return (0);
		e->ptr = head;
		e->magic = *((int*)e->ptr);
		if (head->magic == MH_MAGIC_64 || head->magic == MH_CIGAM_64)
		{
			if (!handle_magic_64(e, head))
				return (0);
		}
		else if (head->magic == MH_MAGIC || head->magic == MH_CIGAM)
			if (!handle_magic_32(e, (void*)head))
				return (0);
	}
	return (1);
}

int			handle_fat_32(t_env *e, struct fat_header *header)
{
	uint32_t				i;
	struct fat_arch			*fat;
	struct mach_header_64	*head;

	fat = (void*)header + sizeof(*header);
	i = -1;
	e->nfat_arch = swap32(header->nfat_arch, e->fat_magic);
	while (++i < swap32(header->nfat_arch, e->fat_magic))
	{
		head = (void*)header + swap32(fat[i].offset, e->fat_magic);
		if (!ft_strncmp((void*)head, ARMAG, SARMAG)
		&& swap32(fat[i].cputype, e->fat_magic) == CPU_TYPE_X86_64)
		{
			e->ptr = head;
			return (handle_archive(e));
		}
		if (head->magic == MH_MAGIC_64 || head->magic == MH_CIGAM_64)
		{
			e->ptr = head;
			e->magic = *((int*)e->ptr);
			return (handle_magic_64(e, head));
		}
	}
	e->mult_arch = 1;
	return (handle_all_arch_32(e, header));
}

static int	handle_all_arch_64(t_env *e, struct fat_header *header)
{
	uint32_t				i;
	struct fat_arch_64		*fat;
	struct mach_header_64	*head;

	fat = (void*)header + sizeof(*header);
	i = -1;
	while (++i < swap32(header->nfat_arch, e->fat_magic))
	{
		head = (void*)header + swap64(fat[i].offset, e->fat_magic);
		if (corrupted_ptr(e, head))
			return (0);
		e->ptr = head;
		e->magic = *((int*)e->ptr);
		if (head->magic == MH_MAGIC_64 || head->magic == MH_CIGAM_64)
		{
			if (!handle_magic_64(e, head))
				return (0);
		}
		else if (head->magic == MH_MAGIC || head->magic == MH_CIGAM)
			if (!handle_magic_32(e, (void*)head))
				return (0);
	}
	return (1);
}

int			handle_fat_64(t_env *e, struct fat_header *header)
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
			return (handle_magic_64(e, head));
		}
	}
	e->mult_arch = 1;
	return (handle_all_arch_64(e, header));
}
