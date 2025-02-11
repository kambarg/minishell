/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkambarb <gkambarb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 17:11:42 by gkambarb          #+#    #+#             */
/*   Updated: 2024/07/18 11:20:16 by gkambarb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*subs;

	if (start > ft_strlen(s))
	{
		subs = (char *)malloc(sizeof(char));
		if (subs == NULL)
			return (NULL);
		subs[0] = '\0';
		return (subs);
	}
	if (len >= ft_strlen(s) - start)
		len = ft_strlen(s) - start;
	subs = (char *)malloc((len + 1) * sizeof(char));
	if (subs == NULL)
		return (NULL);
	ft_strlcpy(subs, s + start, len + 1);
	return (subs);
}
