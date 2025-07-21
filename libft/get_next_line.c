/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkambarb <gkambarb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 21:25:18 by gkambarb          #+#    #+#             */
/*   Updated: 2025/01/06 13:46:48 by gkambarb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*lineappend(char *dst, char *src)
{
	char	*str;

	str = ft_strjoin((const char *)dst, (const char *)src);
	free (dst);
	if (!str)
		return (NULL);
	return (str);
}

static char	*fill_line(int fd, char *buf)
{
	char	*res;
	ssize_t	rc;

	res = ft_strdup(buf);
	rc = 1;
	while (res != NULL && rc != 0 && !ft_strchr(res, '\n'))
	{
		rc = read(fd, buf, BUFFER_SIZE);
		if (rc == -1)
		{
			buf = NULL;
			free (res);
			return (NULL);
		}
		buf[rc] = '\0';
		res = lineappend(res, buf);
	}
	return (res);
}

static char	*split_newline(char *line, char *buf)
{
	char	*res;
	char	*nl;

	nl = ft_strchr(line, '\n');
	if (nl == NULL)
	{
		buf[0] = '\0';
		return (line);
	}
	res = ft_substr(line, 0, ft_strlen(line) - ft_strlen(nl) + 1);
	ft_strlcpy(buf, nl + 1, ft_strlen(nl));
	buf[ft_strlen(nl)] = '\0';
	free (line);
	return (res);
}

char	*get_next_line(int fd)
{
	static char	buf[BUFFER_SIZE + 1];
	char		*line;

	if (BUFFER_SIZE <= 0 || fd <= -1)
		return (NULL);
	line = fill_line(fd, buf);
	if (line && line[0] != '\0')
		line = split_newline(line, buf);
	else if (!line)
		buf[0] = '\0';
	else if (line)
	{
		free(line);
		return (NULL);
	}
	return (line);
}
