/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_32.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhonore <qhonore@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/06 15:04:07 by qhonore           #+#    #+#             */
/*   Updated: 2018/03/09 23:14:15 by qhonore          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

static int	sort_nsyms(t_env *e, struct nlist **tab, char *strtable,\
																uint32_t len)
{
	uint32_t		i;
	struct nlist	*tmp;

	i = 0;
	while (i < len - 1)
	{
		if (corrupted_ptr(e, strtable + swap32(tab[i]->n_un.n_strx, e->magic)))
			return (0);
		if (ft_strcmp(strtable + swap32(tab[i]->n_un.n_strx, e->magic),\
		strtable + swap32(tab[i + 1]->n_un.n_strx, e->magic)) > 0\
		|| (ft_strcmp(strtable + swap32(tab[i]->n_un.n_strx, e->magic),\
		strtable + swap32(tab[i + 1]->n_un.n_strx, e->magic)) == 0\
		&& swap32(tab[i]->n_value, e->magic) > swap32(tab[i + 1]->n_value,\
		e->magic)))
		{
			tmp = tab[i];
			tab[i] = tab[i + 1];
			tab[i + 1] = tmp;
			if (i > 0)
				--i;
		}
		else
			++i;
	}
	return (1);
}

static void	print_nlist(t_env *e, struct nlist *sorted, char type,\
															char *strtable)
{
	print_addr_32(swap32(sorted->n_value, e->magic), sorted->n_type);
	ft_putchar(' ');
	ft_putchar(type);
	ft_putchar(' ');
	ft_putstr(strtable + swap32(sorted->n_un.n_strx, e->magic));
	ft_putchar('\n');
}

static int	print_symtab_command(t_env *e, struct symtab_command *sym,\
																	uint32_t i)
{
	char			*strtable;
	struct nlist	*array;
	struct nlist	*sorted[swap32(sym->nsyms, e->magic)];
	char			type;

	e->sym = sym;
	strtable = e->ptr + swap32(sym->stroff, e->magic);
	array = e->ptr + swap32(sym->symoff, e->magic);
	if (corrupted_ptr(e, strtable) || corrupted_ptr(e, array + sym->nsyms - 1))
		return (0);
	while (++i < swap32(sym->nsyms, e->magic))
		sorted[i] = array + i;
	if (!sort_nsyms(e, sorted, strtable, swap32(sym->nsyms, e->magic)))
		return (0);
	i = -1;
	while (++i < swap32(sym->nsyms, e->magic))
	{
		if (corrupted_ptr(e, sorted[i]))
			return (0);
		if ((type = get_ntype_32(e, sorted[i], sorted[i]->n_type,\
		swap32(sorted[i]->n_value, e->magic))) == '-')
			continue ;
		print_nlist(e, sorted[i], type, strtable);
	}
	return (1);
}

int			handle_magic_32(t_env *e, struct mach_header *header)
{
	uint32_t		i;

	i = -1;
	e->header = header;
	e->lc = e->ptr + sizeof(*header);
	if (corrupted_ptr(e, e->lc) || corrupted_ptr(e, e->header))
		return (0);
	print_filename(e);
	while (++i < swap32(header->ncmds, e->magic))
	{
		if (swap32(e->lc->cmd, e->magic) == LC_SYMTAB)
		{
			if (!print_symtab_command(e, (void*)e->lc, -1))
				return (0);
			break ;
		}
		e->lc = (void*)e->lc + swap32(e->lc->cmdsize, e->magic);
		if (corrupted_ptr(e, e->lc))
			return (0);
	}
	return (1);
}
