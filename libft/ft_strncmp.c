/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkambarb <gkambarb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 10:01:39 by gkambarb          #+#    #+#             */
/*   Updated: 2024/07/18 11:25:10 by gkambarb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t			i;
	unsigned char	*ss1;
	unsigned char	*ss2;

	i = 0;
	ss1 = (unsigned char *)s1;
	ss2 = (unsigned char *)s2;
	while (ss1[i] != '\0' && ss2[i] != '\0' && i < n)
	{
		if (ss1[i] - ss2[i] == 0)
			i++;
		else
			return (ss1[i] - ss2[i]);
	}
	if (ss1[i] == '\0' && ss2[i] != '\0' && i < n)
		return (-1 * ss2[i]);
	else if (ss1[i] != '\0' && ss2[i] == '\0' && i < n)
		return (ss1[i]);
	else
		return (0);
}
