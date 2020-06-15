/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_long.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/05 18:33:38 by ashishae          #+#    #+#             */
/*   Updated: 2020/06/15 15:18:00 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

static int	count_digits_ulong(unsigned long n)
{
	int digits;

	if (n == 0)
		return (1);
	digits = 0;
	while (n != 0)
	{
		n = n / 10;
		digits++;
	}
	return (digits);
}

int			print_long(unsigned long n, char *result)
{
	// char			*result;
	int				digits;
	int				i;

	digits = count_digits_ulong(n);
	i = digits - 1;
	// if (!(result = malloc(sizeof(char) * (digits + 1))))
	// 	return (NULL);
	result[digits] = '\0';
	while (i > -1)
	{
		result[i--] = n % 10 + '0';
		n = n / 10;
	}
	return (digits);
}

size_t	ft_strlcpy(char *dst, char *src, size_t dstsize)
{
	unsigned int i;
	unsigned int limit;

	i = 0;
	limit = (unsigned int)(dstsize - 1);
	if (!dst || !src)
		return (0);
	if (dstsize == 0)
		return (ft_strlen(src));
	while (src[i] != '\0' && i < limit)
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (ft_strlen(src));
}