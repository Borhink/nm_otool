/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhonore <qhonore@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/28 16:48:20 by qhonore           #+#    #+#             */
/*   Updated: 2018/03/05 23:29:19 by qhonore          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

void	sort_nsyms(struct nlist_64 **array, char *stringtable, uint32_t len)
{
	uint32_t		i;
	struct nlist_64	*tmp;

	i = 0;
	while (i < len - 1)
	{
		if (ft_strcmp(stringtable + array[i]->n_un.n_strx,\
		stringtable + array[i + 1]->n_un.n_strx) > 0)
		{
			tmp = array[i];
			array[i] = array[i + 1];
			array[i + 1] = tmp;
			if (i > 0)
				--i;
		}
		else
			++i;
	}
}

void	print_nvalue(uint32_t addr, int def)
{
	const char	*base = "0123456789abcdef";
	char		str[17];
	int			i;

	str[16] = '\0';
	i = 16;
	while (--i >= 8)
	{
		str[i] = def ? base[addr % 16] : ' ';
		addr /= 16;
	}
	str[i] = def ? '1' : ' ';
	while (--i >= 0)
		str[i] = def ? '0' : ' ';
	ft_putstr(str);
}

int		check_names(t_env *e, uint8_t n_sect, char *segname, char *sectname)
{
	struct load_command			*lc;
	struct segment_command_64	*seg;
	struct section_64			*sect;
	uint32_t					i;
	uint32_t					j;

	i = -1;
	lc = e->ptr + sizeof(*e->header);
	while (++i < e->header->ncmds)
	{
		if (lc->cmd == LC_SEGMENT_64)
		{
			seg = (void*)lc;
			sect = (void*)lc + sizeof(*(seg));
			j = -1;
			while (++j < seg->nsects)
			{
				if (n_sect == 0)
				{
					if (!ft_strcmp(sect[j].segname, segname)\
					&& !ft_strcmp(sect[j].sectname, sectname))
						return (1);
					return (0);
				}
				--n_sect;
			}
		}
		lc = (void*)lc + lc->cmdsize;
	}
	return (0);
}

uint32_t	check_sect(t_env *e, uint8_t n_sect)
{
	char	c;

	if (check_names(e, n_sect, SEG_TEXT, SECT_TEXT))
		c = 't';
	else if (check_names(e, n_sect, SEG_DATA, SECT_DATA))
		c = 'd';
	else if (check_names(e, n_sect, SEG_DATA, SECT_BSS))
		c = 'b';
	else
		c = 's';
	return (c);
}

void	print_ntype(t_env *e, struct nlist_64 *sorted, uint32_t n_type, int n_value)
{
	char		type;

	if ((n_type & N_STAB))
		type = '-';
	else if ((n_type & N_TYPE) == N_UNDF && n_value)
		type = 'c';
	else if ((n_type & N_TYPE) == N_UNDF || (n_type & N_TYPE) == N_PBUD)
		type = 'u';
	else if ((n_type & N_TYPE) == N_ABS)
		type = 'a';
	else if ((n_type & N_TYPE) == N_SECT)
		type = check_sect(e, sorted->n_sect - 1);
	else if ((n_type & N_TYPE) == N_INDR)
		type = 'i';
	else
		type = '?';
	if (n_type & N_EXT)
		type = ft_toupper(type);
	ft_putchar(type);
}

void	print_symtab_command(t_env *e, struct symtab_command *sym)
{
	uint32_t		i;
	char			*stringtable;
	struct nlist_64	*array;
	struct nlist_64	*sorted[sym->nsyms];

	e->sym = sym;
	stringtable = e->ptr + sym->stroff;
	array = e->ptr + sym->symoff;
	i = -1;
	while (++i < sym->nsyms)
		sorted[i] = array + i;
	sort_nsyms(sorted, stringtable, sym->nsyms);
	i = -1;
	while (++i < sym->nsyms)
	{
		// printf("nlist_64\n{\n  n_strx: %d,\n  n_type: %d,\n  n_sect: %d,\n  n_desc: %d,\n  n_value: %lld\n}\n",\
		// sorted[i]->n_un.n_strx, sorted[i]->n_type, sorted[i]->n_sect, sorted[i]->n_desc, sorted[i]->n_value);
		print_nvalue(sorted[i]->n_value, ((sorted[i]->n_type & N_TYPE) != N_UNDF));
		ft_putchar(' ');
		print_ntype(e, sorted[i], sorted[i]->n_type, sorted[i]->n_value);
		ft_putchar(' ');
		ft_putstr(stringtable + sorted[i]->n_un.n_strx);
		ft_putchar('\n');
	}
}

void	handle_magic_64(t_env *e, struct mach_header_64 *header)
{
	uint32_t		i;

	i = -1;
	e->header = header;
	e->lc = e->ptr + sizeof(*header);
	while (++i < header->ncmds)
	{
		if (e->lc->cmd == LC_SYMTAB)
		{
			print_symtab_command(e, (void*)e->lc);
			break ;
		}
		e->lc = (void*)e->lc + e->lc->cmdsize;
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

//. >
