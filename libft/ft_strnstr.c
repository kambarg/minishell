/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkambarb <gkambarb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 16:32:03 by gkambarb          #+#    #+#             */
/*   Updated: 2024/07/18 11:24:51 by gkambarb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	little_len;
	size_t	k;
	int		res;

	little_len = ft_strlen(little);
	if (little_len == 0)
		return ((char *)(big));
	if (len == 0)
		return (NULL);
	k = 0;
	while ((big[k] != '\0') && (len >= little_len) && (k <= (len - little_len)))
	{
		res = ft_strncmp(big + k, little, little_len);
		if (res == 0)
			return ((char *)(big + k));
		else
			k++;
	}
	return (NULL);
}
