/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_32.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhonore <qhonore@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/06 15:04:07 by qhonore           #+#    #+#             */
/*   Updated: 2018/03/07 18:23:34 by qhonore          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

static void	sort_nsyms(t_env *e, struct nlist **array, char *stringtable,\
																uint32_t len)
{
	uint32_t		i;
	struct nlist	*tmp;

	i = 0;
	while (i < len - 1)
	{
		if (ft_strcmp(stringtable + swap32(array[i]->n_un.n_strx, e->magic),\
		stringtable + swap32(array[i + 1]->n_un.n_strx, e->magic)) > 0\
		|| (ft_strcmp(stringtable + swap32(array[i]->n_un.n_strx, e->magic),\
		stringtable + swap32(array[i + 1]->n_un.n_strx, e->magic)) == 0\
		&& swap32(array[i]->n_value, e->magic) > swap32(array[i + 1]->n_value,\
		e->magic)))
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

static void	print_nlist(t_env *e, struct nlist *sorted, char type,\
															char *stringtable)
{
	print_addr_32(swap32(sorted->n_value, e->magic), sorted->n_type);
	ft_putchar(' ');
	ft_putchar(type);
	ft_putchar(' ');
	ft_putstr(stringtable + swap32(sorted->n_un.n_strx, e->magic));
	ft_putchar('\n');
}

static void	print_symtab_command(t_env *e, struct symtab_command *sym)
{
	uint32_t		i;
	char			*stringtable;
	struct nlist	*array;
	struct nlist	*sorted[swap32(sym->nsyms, e->magic)];
	char			type;

	e->sym = sym;
	stringtable = e->ptr + swap32(sym->stroff, e->magic);
	array = e->ptr + swap32(sym->symoff, e->magic);
	i = -1;
	while (++i < swap32(sym->nsyms, e->magic))
		sorted[i] = array + i;
	sort_nsyms(e, sorted, stringtable, swap32(sym->nsyms, e->magic));
	i = -1;
	while (++i < swap32(sym->nsyms, e->magic))
	{
		if ((type = get_ntype_32(e, sorted[i], sorted[i]->n_type,\
		swap32(sorted[i]->n_value, e->magic))) == '-')
			continue ;
		print_nlist(e, sorted[i], type, stringtable);
	}
}

void		handle_magic_32(t_env *e, struct mach_header *header)
{
	uint32_t		i;

	i = -1;
	e->header = header;
	e->lc = e->ptr + sizeof(*header);
	print_filename(e);
	while (++i < swap32(header->ncmds, e->magic))
	{
		if (swap32(e->lc->cmd, e->magic) == LC_SYMTAB)
		{
			print_symtab_command(e, (void*)e->lc);
			break ;
		}
		e->lc = (void*)e->lc + swap32(e->lc->cmdsize, e->magic);
	}
}