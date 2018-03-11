/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhonore <qhonore@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/28 16:48:20 by qhonore           #+#    #+#             */
/*   Updated: 2018/03/11 17:50:10 by qhonore          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

int			nm(t_env *e)
{
	if (corrupted_ptr(e, e->ptr))
		return (0);
	e->magic = *((int*)e->ptr);
	e->fat_magic = 0;
	e->mult_arch = 0;
	if (e->magic == MH_MAGIC_64 || e->magic == MH_CIGAM_64)
		return (handle_magic_64(e, e->ptr));
	else if (e->magic == MH_MAGIC || e->magic == MH_CIGAM)
		return (handle_magic_32(e, e->ptr));
	else if (e->magic == FAT_MAGIC || e->magic == FAT_CIGAM)
	{
		e->fat_magic = e->magic;
		return (handle_fat_32(e, e->ptr));
	}
	else if (e->magic == FAT_MAGIC_64 || e->magic == FAT_CIGAM_64)
	{
		e->fat_magic = e->magic;
		return (handle_fat_64(e, e->ptr));
	}
	else if (!ft_strncmp((char*)e->ptr, ARMAG, SARMAG))
		return (handle_archive(e));
	else
		ft_putstr("Format not handled\n");
	return (1);
}

static void	*mmap_file(struct stat *buff, char *path)
{
	void	*ptr;
	int		fd;

	if ((fd = open(path, O_RDONLY)) < 0)
		ft_putstr(RED "Open failure\n" EOC);
	else if (fstat(fd, buff) < 0)
		ft_putstr(RED "Fstat failure\n" EOC);
	else if ((ptr = mmap(0, buff->st_size, PROT_READ,\
	MAP_PRIVATE, fd, 0)) == MAP_FAILED)
		ft_putstr(RED "Mmap failure\n" EOC);
	else
		return (ptr);
	return (NULL);
}

void		parse_args(t_env *e, int ac, char **av)
{
	struct stat	buff;
	int			i;
	void		*ptr;

	i = 0;
	while (++i < ac)
	{
		if ((ptr = mmap_file(&buff, av[i])))
		{
			e->ptr = ptr;
			e->filename = av[i];
			e->eof = ptr + buff.st_size;
			if (!nm(e))
				ft_putstr(RED "Corrupted file !\n" EOC);
			if (munmap(ptr, buff.st_size) != 0)
				ft_putstr(RED "Munmap failure\n" EOC);
		}
	}
}

int			main(int ac, char **av)
{
	t_env		e;

	if (ac < 2)
		ft_putstr(YELLOW "usage: ./ft_nm [path]\n" EOC);
	else
	{
		ft_memset(&e, 0, sizeof(t_env));
		if (ac > 2)
			e.mult = 1;
		parse_args(&e, ac, av);
	}
	return (0);
}
