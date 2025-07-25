/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract_word.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 11:00:02 by root              #+#    #+#             */
/*   Updated: 2025/07/25 11:01:21 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	skip_quoted_assignment_value(char *input, int *i)
{
	char	quote;

	quote = input[*i];
	(*i)++;
	while (input[*i] && input[*i] != quote)
		(*i)++;
	if (input[*i] == quote)
		(*i)++;
}

static void	parse_assignment_value(char *input, int *i)
{
	while (input[*i] && !is_whitespace(input[*i]) && !is_operator_char(input[*i]))
	{
		if (is_quotes(input[*i]))
			skip_quoted_assignment_value(input, i);
		else
			(*i)++;
	}
}

static void	skip_word_body(char *input, int *i, int start)
{
	while (input[*i] && !is_whitespace(input[*i]) 
		&& !is_operator_char(input[*i]) && !is_quotes(input[*i]))
	{
		if (input[*i] == '=' && *i > start)
		{
			(*i)++;
			parse_assignment_value(input, i);
			break;
		}
		(*i)++;
	}
}

static char	*copy_word_content(char *input, int start, int end)
{
	int		len;
	char	*content;

	len = end - start;
	if (len == 0)
		return (ft_strdup(""));
	content = (char *)malloc(len + 1);
	if (!content)
		return (NULL);
	ft_strlcpy(content, input + start, len + 1);
	return (content);
}

char	*extract_word_content(char *input, int *i)
{
	int	start;

	start = *i;
	skip_word_body(input, i, start);
	return (copy_word_content(input, start, *i));
}
