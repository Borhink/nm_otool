/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhonore <qhonore@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/28 16:48:20 by qhonore           #+#    #+#             */
/*   Updated: 2018/03/07 17:26:12 by qhonore          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

void	nm(t_env *e)
{
	e->magic = *((int*)e->ptr);
	e->fat_magic = 0;
	e->mult_arch = 0;
	if (e->magic == MH_MAGIC_64 || e->magic == MH_CIGAM_64)
		handle_magic_64(e, e->ptr);
	else if (e->magic == MH_MAGIC || e->magic == MH_CIGAM)
		handle_magic_32(e, e->ptr);
	else if (e->magic == FAT_MAGIC || e->magic == FAT_CIGAM)
	{
		e->fat_magic = e->magic;
		handle_fat_32(e, e->ptr);
	}
	else if (e->magic == FAT_MAGIC_64 || e->magic == FAT_CIGAM_64)
	{
		e->fat_magic = e->magic;
		handle_fat_64(e, e->ptr);
	}
	else
		ft_putstr("Format not handled\n");
}

void	parse_args(t_env *e, int ac, char **av)
{
	int			fd;
	struct stat	buff;
	int			i;

	i = 0;
	while (++i < ac)
	{
		if ((fd = open(av[i], O_RDONLY)) < 0)
			ft_putstr(RED "Open failure\n" EOC);
		else if (fstat(fd, &buff) < 0)
			ft_putstr(RED "Fstat failure\n" EOC);
		else if ((e->ptr = mmap(0, buff.st_size, PROT_READ,\
		MAP_PRIVATE, fd, 0)) == MAP_FAILED)
			ft_putstr(RED "Mmap failure\n" EOC);
		else
		{
			e->filename = av[i];
			nm(e);
			if (munmap(e->ptr, buff.st_size) != 0)
				ft_putstr(RED "Munmap failure\n" EOC);
		}
	}
}

int		main(int ac, char **av)
{
	t_env		e;

	if (ac < 2)
		ft_putstr(YELLOW "usage: ./ft_nm [path]\n" EOC);
	else
	{
		ft_memset(&e, 0, sizeof(t_env));
		if (ac > 2)
			e.mult = 1;
		e.first = 1;
		parse_args(&e, ac, av);
	}
	return (0);
}

/*
** /usr/bin/audiodevice (for architecture ppc):
** /usr/bin/audiodevice (for architecture i386):
** ft_nm:
*/
