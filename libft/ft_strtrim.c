/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkambarb <gkambarb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 17:19:49 by gkambarb          #+#    #+#             */
/*   Updated: 2025/01/19 18:47:02 by gkambarb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*case_strcpy(char const *s1)
{
	int		i;
	char	*res;

	res = (char *)malloc((ft_strlen(s1) + 1) * sizeof(char));
	if (res == NULL)
		return (NULL);
	i = 0;
	while (s1[i] != '\0')
	{
		res[i] = s1[i];
		i++;
	}
	res[i] = '\0';
	return (res);
}

static int	find_start(char const *s1, char const *set)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (set[i] != '\0')
	{
		if (s1[j] != set[i])
			i++;
		else
		{
			j++;
			i = 0;
		}
	}
	return (j);
}

static int	find_end(char const *s1, char const *set, int i, int j)
{
	while (set[i] != '\0')
	{
		if (s1[j] != set[i])
			i++;
		else
		{
			j--;
			i = 0;
		}
	}
	return (j);
}

static char	*case_strt(char const *s1, int start, int end)
{
	int		i;
	char	*res;

	res = (char *)malloc((end - start + 1 + 1) * sizeof(char));
	if (res == NULL)
		return (NULL);
	i = 0;
	while (i < end - start + 1)
	{
		res[i] = s1[i + start];
		i++;
	}
	res[i] = '\0';
	return (res);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	int		i;
	int		j;
	int		start;
	int		end;
	char	*res;

	if (ft_strncmp(s1, "", 1) == 0 || ft_strncmp(set, "", 1) == 0)
	{
		res = case_strcpy(s1);
		return (res);
	}
	start = find_start(s1, set);
	if (s1[start] == '\0')
	{
		res = (char *)ft_calloc(1, sizeof(char));
		if (res == NULL)
			return (NULL);
		return (res);
	}
	i = 0;
	j = ft_strlen(s1) - 1;
	end = find_end(s1, set, i, j);
	res = case_strt(s1, start, end);
	return (res);
}
