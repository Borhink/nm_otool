/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_64.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhonore <qhonore@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/06 15:04:07 by qhonore           #+#    #+#             */
/*   Updated: 2018/03/07 17:45:31 by qhonore          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

static void	sort_nsyms(struct nlist_64 **array, char *stringtable, uint32_t len)
{
	uint32_t		i;
	struct nlist_64	*tmp;

	i = 0;
	while (i < len - 1)
	{
		if (ft_strcmp(stringtable + array[i]->n_un.n_strx,\
		stringtable + array[i + 1]->n_un.n_strx) > 0\
		|| (ft_strcmp(stringtable + array[i]->n_un.n_strx,\
		stringtable + array[i + 1]->n_un.n_strx) == 0\
		&& array[i]->n_value > array[i + 1]->n_value))
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

static void	print_nlist(struct nlist_64 *sorted, char type, char *stringtable)
{
	print_addr_64(sorted->n_value, sorted->n_type);
	ft_putchar(' ');
	ft_putchar(type);
	ft_putchar(' ');
	ft_putstr(stringtable + sorted->n_un.n_strx);
	ft_putchar('\n');
}

static void	print_symtab_command(t_env *e, struct symtab_command *sym)
{
	uint32_t		i;
	char			*stringtable;
	struct nlist_64	*array;
	struct nlist_64	*sorted[sym->nsyms];
	char			type;

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
		if ((type = get_ntype_64(e, sorted[i], sorted[i]->n_type,\
		sorted[i]->n_value)) == '-')
			continue ;
		print_nlist(sorted[i], type, stringtable);
	}
}

void		handle_magic_64(t_env *e, struct mach_header_64 *header)
{
	uint32_t		i;

	i = -1;
	e->header64 = header;
	e->lc = e->ptr + sizeof(*header);
	print_filename(e);
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
