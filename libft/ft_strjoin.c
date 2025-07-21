/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkambarb <gkambarb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 12:53:50 by gkambarb          #+#    #+#             */
/*   Updated: 2024/07/18 11:27:52 by gkambarb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*sjoin;
	int		sjoin_len;
	int		i;

	sjoin_len = ft_strlen(s1) + ft_strlen(s2) + 1;
	sjoin = (char *)malloc(sjoin_len * sizeof(char));
	if (sjoin == NULL)
		return (NULL);
	i = 0;
	while (*s1 != '\0')
	{
		sjoin[i] = *s1;
		s1++;
		i++;
	}
	while (*s2 != '\0')
	{
		sjoin[i] = *s2;
		s2++;
		i++;
	}
	sjoin[i] = '\0';
	return (sjoin);
}
