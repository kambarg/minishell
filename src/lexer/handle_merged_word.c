/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_merged_word.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 10:29:05 by root              #+#    #+#             */
/*   Updated: 2025/07/25 11:24:23 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// Handle merged word (adjacent quoted/unquoted segments)
int	handle_merged_word(char *input, int *i, t_token **tokens)
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

            if (quote_char == '\'')
                current_quote_type = QUOTE_SINGLE;
            else if (quote_char == '"')
                current_quote_type = QUOTE_DOUBLE;

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
