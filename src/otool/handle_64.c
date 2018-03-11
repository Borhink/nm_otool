/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_64.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhonore <qhonore@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/06 15:04:07 by qhonore           #+#    #+#             */
/*   Updated: 2018/03/11 20:33:48 by qhonore          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

static void	print_addr(uint64_t addr)
{
	const char	*base = "0123456789abcdef";
	char		str[17];
	int			i;

	str[16] = '\0';
	i = 16;
	while (--i >= 0)
	{
		str[i] = base[addr % 16];
		addr /= 16;
	}
	ft_putstr(str);
	ft_putchar('\t');
}

static int	print_section(t_env *e, struct section_64 sect, char *segname,\
																char *sectname)
{
	const char		*base = "0123456789abcdef";
	uint32_t		i;
	unsigned char	*ptr;

	ft_putstr("Contents of (");
	ft_putstr(segname);
	ft_putstr(",");
	ft_putstr(sectname);
	ft_putstr(") section\n");
	i = -1;
	while (++i < swap64(sect.size, e->magic))
	{
		ptr = (void*)e->header64 + swap32(sect.offset, e->magic) + i;
		if (corrupted_ptr(e, ptr))
			return (0);
		if (i % 16 == 0)
			print_addr(swap64(sect.addr, e->magic) + i);
		ft_putchar(base[*ptr / 16]);
		ft_putchar(base[*ptr % 16]);
		ft_putchar(' ');
		if ((i + 1) % 16 == 0 || (i + 1) == swap64(sect.size, e->magic))
			ft_putchar('\n');
	}
	return (1);
}

static int	print_segment(t_env *e, struct segment_command_64 *seg)
{
	struct section_64	*sect;
	uint32_t			i;

	i = -1;
	sect = (void*)seg + sizeof(*(seg));
	while (++i < swap32(seg->nsects, e->magic))
	{
		if (corrupted_ptr(e, sect + i))
			return (0);
		if (!ft_strcmp(sect[i].segname, SEG_TEXT)\
		&& !ft_strcmp(sect[i].sectname, SECT_TEXT))
			if (!print_section(e, sect[i], SEG_TEXT, SECT_TEXT))
				return (0);
	}
	return (1);
}

int			handle_magic_64(t_env *e, struct mach_header_64 *header)
{
	uint32_t		i;

	i = -1;
	e->header64 = header;
	e->lc = e->ptr + sizeof(*header);
	if (corrupted_ptr(e, e->lc) || corrupted_ptr(e, e->header64))
		return (0);
	print_filename(e);
	while (++i < swap32(header->ncmds, e->magic))
	{
		if (swap32(e->lc->cmd, e->magic) == LC_SEGMENT_64)
		{
			if (!print_segment(e, (void*)e->lc))
				return (0);
		}
		e->lc = (void*)e->lc + swap32(e->lc->cmdsize, e->magic);
		if (corrupted_ptr(e, e->lc))
			return (0);
	}
	return (1);
}
