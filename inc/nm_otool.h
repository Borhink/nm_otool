/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm_otool.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhonore <qhonore@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/12 17:44:40 by qhonore           #+#    #+#             */
/*   Updated: 2018/03/06 22:13:29 by qhonore          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NM_OTOOL_H
# define NM_OTOOL_H

# include <sys/mman.h>
# include <sys/stat.h>
# include <mach-o/loader.h>
# include <mach-o/nlist.h>
# include <mach-o/fat.h>
# include <mach/machine.h>
# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
#include <stdio.h>

# define GREEN "\033[32m"
# define RED "\033[31m"
# define YELLOW "\033[33m"
# define MAGENTA "\033[35m"
# define CYAN "\033[36m"
# define EOC "\033[0m"

typedef struct s_env	t_env;

struct	s_env
{
	void					*ptr;
	struct load_command		*lc;
	struct symtab_command	*sym;
	struct mach_header_64	*header64;
	struct mach_header		*header;
	char					**sectnames;
	char					**segnames;
	int						nsect;
};

void		nm(t_env *e);

void		handle_magic_64(t_env *e, struct mach_header_64 *header);
void		print_addr_64(uint64_t addr, uint8_t n_type);
char		get_ntype_64(t_env *e, struct nlist_64 *sorted, uint32_t n_type,\
																int n_value);

void		handle_magic_32(t_env *e, struct mach_header *header);
void		print_addr_32(uint32_t addr, uint8_t n_type);
char		get_ntype_32(t_env *e, struct nlist *sorted, uint32_t n_type,\
																int n_value);

size_t		ft_strlen(char *s);
void		ft_putchar(char c);
void		ft_putstr(char *s);
void		*ft_memset(void *s, int c, size_t n);
int			ft_strcmp(const char *s1, const char *s2);
int			ft_toupper(int c);
uint32_t	swap_bin32(uint32_t n, unsigned int magic_number);
uint64_t	swap_bin64(uint64_t n, unsigned int magic_number);

#endif
