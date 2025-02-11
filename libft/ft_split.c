/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkambarb <gkambarb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 13:36:45 by gkambarb          #+#    #+#             */
/*   Updated: 2024/07/18 11:33:13 by gkambarb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	count(char const *s, char c)
{
	int	i;
	int	countw;

	countw = 0;
	i = 0;
	while (s[i] != '\0')
	{
		if (s[i] != c && (s[i + 1] == c || s[i + 1] == '\0'))
			countw++;
		i++;
	}
	return (countw);
}

static char	**my_free(char **words, int j)
{
	while (j > 0)
	{
		free(words[j - 1]);
		j--;
	}
	free(words);
	return (NULL);
}

static char	*proccess(char const *s, char c, int i)
{
	int		ws;
	int		we;
	char	*word;

	ws = i;
	while (s[i] != c && s[i] != '\0')
		i++;
	we = i - 1;
	word = (char *)malloc((i - ws + 1) * sizeof(char));
	if (!word)
		return (NULL);
	word[we - ws + 1] = '\0';
	while (we > ws - 1)
	{
		word[we - ws] = s[we];
		we--;
	}
	return (word);
}

char	**ft_split(char const *s, char c)
{
	int		i;
	int		j;
	char	**words;

	words = (char **)malloc((count(s, c) + 1) * sizeof(char *));
	if (!words)
		return (NULL);
	i = 0;
	j = 0;
	while (s[i] != '\0')
	{
		if (s[i] == c)
			i++;
		else
		{
			words[j] = proccess(s, c, i);
			if (!words[j])
				return (my_free(words, j));
			i = i + ft_strlen(words[j]);
			j++;
		}
	}
	words[j] = NULL;
	return (words);
}
