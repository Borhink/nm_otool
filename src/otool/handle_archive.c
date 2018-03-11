/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_archive.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhonore <qhonore@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/09 15:15:41 by qhonore           #+#    #+#             */
/*   Updated: 2018/03/11 21:10:00 by qhonore          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

static void	sort_nsyms(struct ranlib **array, uint32_t sym_nb)
{
	uint32_t		i;
	struct ranlib	*tmp;

	i = 0;
	while (i < sym_nb - 1)
	{
		if (array[i]->ran_off > array[i + 1]->ran_off)
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

static int	sym_already_in(struct ranlib **array, struct ranlib *new,\
																uint32_t size)
{
	uint32_t		i;

	i = -1;
	while (++i < size)
		if (array[i]->ran_off == new->ran_off)
			return (1);
	return (0);
}

static void	order_archive(t_env *e, struct ranlib **sorted, uint32_t sym_nb,\
															struct ranlib *ran)
{
	uint32_t		i;
	uint32_t		j;

	i = -1;
	j = 0;
	while (++i < sym_nb)
		if (!sym_already_in(sorted, ran + i, j))
			sorted[j++] = ran + i;
	e->sym_nb = j;
	if (e->sym_nb > 1)
		sort_nsyms(sorted, e->sym_nb);
	ft_putstr("Archive : ");
	ft_putstr(e->filename);
	ft_putchar('\n');
}

static int	print_archive(t_env *e, uint32_t sym_nb, struct ranlib *ran)
{
	struct ranlib	*sorted[sym_nb];
	int				len;
	void			*start;
	struct ar_hdr	*ar;
	uint32_t		i;

	order_archive(e, sorted, sym_nb, ran);
	i = -1;
	start = e->ptr;
	while (++i < e->sym_nb)
	{
		ar = start + sorted[i]->ran_off;
		if (corrupted_ptr(e, ar))
			return (0);
		len = ft_atoi(ft_strchr(ar->ar_name, '/') + 1);
		ft_putstr(e->filename);
		ft_putchar('(');
		ft_putstr((void*)ar + sizeof(*ar));
		ft_putstr("):\n");
		e->ptr = (void*)ar + sizeof(*ar) + len;
		otool(e);
	}
	return (1);
}

int			handle_archive(t_env *e)
{
	int				len;
	int				sym_nb;
	void			*symtable;
	struct ranlib	*ran;

	e->archive = 1;
	len = ft_atoi(ft_strchr(e->ptr, '/') + 1);
	symtable = e->ptr + SARMAG + sizeof(struct ar_hdr) + len;
	ran = symtable + sizeof(uint32_t);
	if (corrupted_ptr(e, symtable))
		return (0);
	sym_nb = (*(int*)symtable) / sizeof(struct ranlib);
	if (corrupted_ptr(e, ran + sym_nb) || !print_archive(e, sym_nb, ran))
		return (0);
	return (1);
}
