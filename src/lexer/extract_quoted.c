/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract_quoted.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 11:07:41 by root              #+#    #+#             */
/*   Updated: 2025/07/25 11:07:50 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// Extract quoted string content (without quotes) 
char	*extract_quoted_content(char *input, int *i)
{
	char	quote_char;
	int		start;
	int		len;
	char	*content;

	quote_char = input[*i];
	(*i)++;
	start = *i;
	while (input[*i] && input[*i] != quote_char)
		(*i)++;
	if (!input[*i])
	{
		print_error(NULL, "syntax error: unclosed quotes");
		return (NULL);
	}
	len = *i - start;
	(*i)++;
	content = (char *)malloc(len + 1);
	if (!content)
		return (NULL);
	ft_strlcpy(content, input + start, len + 1);
	return (content);
}
