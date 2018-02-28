/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhonore <qhonore@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/28 16:48:20 by qhonore           #+#    #+#             */
/*   Updated: 2018/02/28 18:36:55 by qhonore          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

void	print_symtab_command(t_env *e, struct symtab_command *sym)
{
	uint32_t		i;
	char			*stringtable;
	struct nlist_64	*array;

	stringtable = e->ptr + sym->stroff;
	array = e->ptr + sym->symoff;
	i = -1;
	while (++i < sym->nsyms)
	{
		ft_putstr(stringtable + array[i].n_un.n_strx);
		ft_putchar('\n');
	}
}

void	handle_magic_64(t_env *e, struct mach_header_64 *header)
{
	struct load_command		*lc;
	uint32_t				i;

	i = -1;
	lc = e->ptr + sizeof(*header);
	while (++i < header->ncmds)
	{
		if (lc->cmd == LC_SYMTAB)
		{
			print_symtab_command(e, (void*)lc);
			break ;
		}
		lc = (void*)lc + lc->cmdsize;
	}
}

void	nm(t_env *e)
{
	uint32_t	magic;

	magic = *((int*)e->ptr);
	// gestion FAT_MAGIC && FAT_CIGAM
	if (magic == MH_MAGIC_64)
		handle_magic_64(e, e->ptr);
}

int		main(int ac, char **av)
{
	int			fd;
	struct stat	buff;
	t_env		e;

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

//. >
