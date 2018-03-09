/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_archive.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhonore <qhonore@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/09 15:15:41 by qhonore           #+#    #+#             */
/*   Updated: 2018/03/09 22:10:17 by qhonore          ###   ########.fr       */
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
}

static void	print_archive(t_env *e, uint32_t sym_nb, struct ranlib *ran)
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
		len = ft_atoi(ft_strchr(ar->ar_name, '/') + 1);
		ft_putchar('\n');
		ft_putstr(e->filename);
		ft_putchar('(');
		ft_putstr((void*)ar + sizeof(*ar));
		ft_putstr("):\n");
		e->ptr = (void*)ar + sizeof(*ar) + len;
		nm(e);
	}
}

int			handle_archive(t_env *e)
{
	int				len;
	int				sym_nb;
	void			*symtable;
	struct ranlib	*ran;

	len = ft_atoi(ft_strchr(e->ptr, '/') + 1);
	symtable = e->ptr + SARMAG + sizeof(struct ar_hdr) + len;
	ran = symtable + sizeof(uint32_t);
	sym_nb = (*(int*)symtable) / sizeof(struct ranlib);
	print_archive(e, sym_nb, ran);
	return (1);
}
