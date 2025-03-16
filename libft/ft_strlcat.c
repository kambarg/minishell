/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkambarb <gkambarb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 12:54:43 by gkambarb          #+#    #+#             */
/*   Updated: 2024/07/18 11:27:37 by gkambarb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dest, const char *src, size_t size)
{
	size_t	i;
	size_t	res;

	if (size >= ft_strlen(dest) + 1)
	{
		res = ft_strlen(dest) + ft_strlen(src);
		i = 0;
		while (dest[i] != '\0')
		{
			i++;
		}
		while (i < size - 1 && *src != '\0')
		{
			dest[i] = *src;
			src++;
			i++;
		}
		dest[i] = '\0';
		return (res);
	}
	else
		return (size + ft_strlen(src));
}
