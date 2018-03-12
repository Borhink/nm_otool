/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhonore <qhonore@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/28 16:48:20 by qhonore           #+#    #+#             */
/*   Updated: 2018/03/12 19:30:25 by qhonore          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

int		ft_atoi(const char *str)
{
	int		nb;
	int		neg;

	nb = 0;
	neg = 0;
	while (*str == '\n' || *str == '\v' || *str == ' '
			|| *str == '\t' || *str == '\r' || *str == '\f')
		str++;
	if (*str == '-')
		neg = 1;
	if (*str == '-' || *str == '+')
		str++;
	while (*str >= '0' && *str <= '9')
	{
		nb = nb * 10 + (*str - '0');
		str++;
	}
	return (neg ? -nb : nb);
}

char	*ft_strchr(const char *s, int c)
{
	int		i;

	i = 0;
	if (c == 0)
		return ((char*)(s + ft_strlen(s)));
	while (s[i] != '\0')
	{
		if (s[i] == (char)c)
			return ((char*)(s + i));
		i++;
	}
	return (NULL);
}

int		corrupted_ptr(t_env *e, void *ptr)
{
	if (ptr > e->eof)
		return (1);
	return (0);
}
