/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm_otool.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhonore <qhonore@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/12 17:44:40 by qhonore           #+#    #+#             */
/*   Updated: 2018/02/28 18:37:05 by qhonore          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NM_OTOOL_H
# define NM_OTOOL_H

# include <sys/mman.h>
# include <sys/stat.h>
# include <mach-o/loader.h>
# include <mach-o/nlist.h>
# include <mach-o/fat.h>
# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>

# define GREEN "\033[32m"
# define RED "\033[31m"
# define YELLOW "\033[33m"
# define MAGENTA "\033[35m"
# define CYAN "\033[36m"
# define EOC "\033[0m"

typedef struct s_env	t_env;

struct	s_env
{
	void	*ptr;
};

void	nm(t_env *e);

size_t	ft_strlen(char *s);
void	ft_putchar(char c);
void	ft_putstr(char *s);

#endif
