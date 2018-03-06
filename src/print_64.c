/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_64.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhonore <qhonore@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/06 15:06:08 by qhonore           #+#    #+#             */
/*   Updated: 2018/03/06 22:14:23 by qhonore          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

void			print_addr_64(uint64_t addr, uint8_t n_type)
{
	const char	*base = "0123456789abcdef";
	int			def;
	char		str[17];
	int			i;

	def = ((n_type & N_TYPE) != N_UNDF);
	str[16] = '\0';
	i = 16;
	while (--i >= 0)
	{
		str[i] = def ? base[addr % 16] : ' ';
		addr /= 16;
	}
	ft_putstr(str);
}

static int	check_names2(struct section_64 sect, char *segname, char *sectname)
{
	if (!ft_strcmp(sect.segname, segname)\
	&& !ft_strcmp(sect.sectname, sectname))
		return (1);
	return (0);
}

static int	check_names(t_env *e, uint8_t n_sect, char *segname, char *sectname)
{
	struct load_command			*lc;
	struct segment_command_64	*seg;
	struct section_64			*sect;
	uint32_t					i;
	uint32_t					j;

	i = -1;
	lc = e->ptr + sizeof(*e->header64);
	while (++i < e->header64->ncmds)
	{
		if (lc->cmd == LC_SEGMENT_64)
		{
			seg = (void*)lc;
			sect = (void*)lc + sizeof(*(seg));
			j = -1;
			while (++j < seg->nsects)
			{
				if (n_sect == 0)
					return (check_names2(sect[j], segname, sectname));
				--n_sect;
			}
		}
		lc = (void*)lc + lc->cmdsize;
	}
	return (-1);
}

static uint32_t	check_sect(t_env *e, uint8_t n_sect)
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

char		get_ntype_64(t_env *e, struct nlist_64 *sorted, uint32_t n_type,\
																	int n_value)
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
	return (type);
}
