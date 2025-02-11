/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkambarb <gkambarb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 18:59:15 by gkambarb          #+#    #+#             */
/*   Updated: 2024/07/18 11:30:11 by gkambarb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *src)
{
	char	*arr;
	int		arr_len;
	int		i;

	arr_len = ft_strlen(src) + 1;
	arr = (char *) malloc(arr_len * sizeof(char));
	if (arr == NULL)
		return (NULL);
	else
	{
		i = 0;
		while (i < arr_len - 1)
		{
			arr[i] = src[i];
			i++;
		}
		arr[i] = '\0';
		return (arr);
	}
}
