/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 10:15:12 by gkambarb          #+#    #+#             */
/*   Updated: 2025/07/25 10:13:59 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// Extract quoted string content (without quotes)
static char	*extract_quoted_content(char *input, int *i)
{
	char	quote_char;
	int		start;
	int		len;
	char	*content;

	quote_char = input[*i];
	(*i)++; // skip opening quote
	start = *i;
	while (input[*i] && input[*i] != quote_char)
		(*i)++;
	if (!input[*i])
	{
		print_error(NULL, "syntax error: unclosed quotes");
		return (NULL);
	}
	len = *i - start;
	(*i)++; // skip closing quote
	content = (char *)malloc(len + 1);
	if (!content)
		return (NULL);
	ft_strlcpy(content, input + start, len + 1);
	return (content);
}

// Extract unquoted word content
static char	*extract_word_content(char *input, int *i)
{
	int		start;
	int		len;
	char	*content;

	start = *i;
	while (input[*i] && !is_whitespace(input[*i]) 
		&& !is_operator_char(input[*i]) && !is_quotes(input[*i]))
	{
		// Handle assignment values like var=value
		if (input[*i] == '=' && *i > start)
		{
			(*i)++;
			// Parse assignment value which can contain quotes
			while (input[*i] && !is_whitespace(input[*i]) 
				&& !is_operator_char(input[*i]))
			{
				if (is_quotes(input[*i]))
				{
					char quote = input[*i];
					(*i)++;
					while (input[*i] && input[*i] != quote)
						(*i)++;
					if (input[*i] == quote)
						(*i)++;
				}
				else
					(*i)++;
			}
			break;
		}
		(*i)++;
	}
	len = *i - start;
	if (len == 0)
		return (ft_strdup(""));
	content = (char *)malloc(len + 1);
	if (!content)
		return (NULL);
	ft_strlcpy(content, input + start, len + 1);
	return (content);
}

// Handle merged word (adjacent quoted/unquoted segments)
static int	handle_merged_word(char *input, int *i, t_token **tokens)
{
	char	*result;
	char	*segment;
	char	*temp;
	int		start_pos;
	int		final_quote_type;
	int		segment_count;
	int		first_quote_type;

	start_pos = *i;
	result = ft_strdup("");
	if (!result)
		return (0);
	
	final_quote_type = QUOTE_NONE;
	segment_count = 0;
	first_quote_type = QUOTE_NONE;
		
	// Process all adjacent segments until whitespace or operator
	while (input[*i] && !is_whitespace(input[*i]) && !is_operator_char(input[*i]))
	{
		int current_quote_type;
		
		if (is_quotes(input[*i]))
		{
			char quote_char = input[*i];
			current_quote_type = (quote_char == '\'') ? QUOTE_SINGLE : QUOTE_DOUBLE;
			segment = extract_quoted_content(input, i);
		}
		else
		{
			current_quote_type = QUOTE_NONE;
			segment = extract_word_content(input, i);
		}
			
		if (!segment)
		{
			free(result);
			return (0);
		}
		
		// Track quote type: if only one segment and it's quoted, preserve its type
		if (segment_count == 0)
			first_quote_type = current_quote_type;
		
		segment_count++;
		
		// Concatenate segment to result
		temp = ft_strjoin(result, segment);
		free(result);
		free(segment);
		if (!temp)
			return (0);
		result = temp;
	}
	
	// Determine final quote type:
	// - If single quoted segment, preserve its quote type
	// - If mixed segments, use QUOTE_NONE
	if (segment_count == 1 && first_quote_type != QUOTE_NONE)
		final_quote_type = first_quote_type;
	else
		final_quote_type = QUOTE_NONE;
	
	// Only add token if we actually parsed something
	if (*i > start_pos)
	{
		add_token(tokens, create_token(result, T_WORD, final_quote_type));
		return (1);
	}
	
	free(result);
	return (1);
}

// Note: handle_merged_word both in quotes and unquoted segments
t_token	*lexer(char *input)
{
	int		i;
	t_token	*tokens;

	i = 0;
	tokens = NULL;
	while (input[i])
	{
		if (is_whitespace(input[i]))
			i++;
		else if (is_operator_char(input[i]))
		{
			if (!handle_operator(input, &i, &tokens))
				return (free_tokens(tokens), NULL);
			i++;
		}
		else
		{
			if (!handle_merged_word(input, &i, &tokens))
				return (free_tokens(tokens), NULL);
		}
	}
	return (tokens);
}
