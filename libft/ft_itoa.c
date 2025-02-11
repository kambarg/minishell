/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkambarb <gkambarb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 13:11:17 by gkambarb          #+#    #+#             */
/*   Updated: 2024/07/18 11:58:46 by gkambarb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static	char	*ft_strcpy(char *dest, char *src)
{
	int	i;

	i = 0;
	while (*src != '\0')
	{
		dest[i] = *src;
		i++;
		src++;
	}
	dest[i] = '\0';
	return (dest);
}

static int	len(int n)
{
	int	i;
	int	ndiv;

	i = 0;
	ndiv = n;
	if (n == INT_MIN)
		return (11);
	else if (n == 0)
		return (1);
	else
	{
		while (ndiv != 0)
		{
			ndiv = ndiv / 10;
			i++;
		}
		if (n > 0)
			return (i);
		else
			return (i + 1);
	}
}

static int	signflag(int n)
{
	if (n > INT_MIN && n < 0)
		return (-1);
	else
		return (1);
}

char	*ft_itoa(int n)
{
	char	*res;
	int		i;

	i = len(n);
	res = (char *)malloc((i + 1) * sizeof(char));
	if (res == NULL)
		return (NULL);
	if (n == INT_MIN)
		ft_strcpy(res, "-2147483648");
	else if (n == 0)
		ft_strcpy(res, "0");
	else
	{
		if (signflag(n) < 0)
			res[0] = '-';
		*(res + i) = '\0';
		while (n != 0)
		{
			*(res + i - 1) = ((signflag(n) * n) % 10) + 48;
			n = n / 10;
			i--;
		}
	}
	return (res);
}
