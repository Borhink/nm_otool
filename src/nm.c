/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhonore <qhonore@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/28 16:48:20 by qhonore           #+#    #+#             */
/*   Updated: 2018/03/06 21:57:03 by qhonore          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

void	handle_fat_32(t_env *e, struct fat_header *header)
{
	uint32_t				i;
	struct fat_arch			*fat64;
	struct mach_header_64	*head;

	fat64 = (void*)header + sizeof(*header);
	i = -1;
	while (++i < swap_bin32(header->nfat_arch, *((int*)e->ptr)))
	{
		head = (void*)header + swap_bin32(fat64[i].offset, *((int*)e->ptr));
		if (head->magic == MH_MAGIC_64 || head->magic == MH_CIGAM_64)
		{
			e->ptr = head;
			handle_magic_64(e, head);
			break ;
		}
	}
}

void	handle_fat_64(t_env *e, struct fat_header *header)
{
	uint32_t				i;
	struct fat_arch_64		*fat64;
	struct mach_header_64	*head;

	fat64 = (void*)header + sizeof(*header);
	i = -1;
	while (++i < swap_bin32(header->nfat_arch, *((int*)e->ptr)))
	{
		head = (void*)header + swap_bin32(fat64[i].offset, *((int*)e->ptr));
		if (head->magic == MH_MAGIC_64 || head->magic == MH_CIGAM_64)
		{
			e->ptr = head;
			handle_magic_64(e, head);
			break ;
		}
	}
}

void	nm(t_env *e)
{
	uint32_t	magic;

	magic = *((int*)e->ptr);
	if (magic == MH_MAGIC_64 || magic == MH_CIGAM_64)
		handle_magic_64(e, e->ptr);
	else if (magic == MH_MAGIC || magic == MH_CIGAM)
		handle_magic_32(e, e->ptr);
	else if (magic == FAT_MAGIC || magic == FAT_CIGAM)
		handle_fat_32(e, e->ptr);
	else if (magic == FAT_MAGIC_64 || magic == FAT_CIGAM_64)
		handle_fat_64(e, e->ptr);
	else
		ft_putstr("Format not handled\n");
}


int		main(int ac, char **av)
{
	int			fd;
	struct stat	buff;
	t_env		e;

	ft_memset(&e, 0, sizeof(t_env));
	if (ac < 2)
		ft_putstr(YELLOW "usage: ./ft_nm [path]\n" EOC);
	else if ((fd = open(av[1], O_RDONLY)) < 0)
		ft_putstr(RED "Open failure\n" EOC);
	else if (fstat(fd, &buff) < 0)
		ft_putstr(RED "Fstat failure\n" EOC);
	else if ((e.ptr = mmap(0, buff.st_size, PROT_READ,\
	MAP_PRIVATE, fd, 0)) == MAP_FAILED)
		ft_putstr(RED "Mmap failure\n" EOC);
	else
	{
		nm(&e);
		if (munmap(e.ptr, buff.st_size) != 0)
			ft_putstr(RED "Munmap failure\n" EOC);
	}
	return (0);
}

// /usr/bin/audiodevice = big 32bit
