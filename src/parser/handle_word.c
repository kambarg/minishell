/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_word.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkambarb <gkambarb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 11:17:46 by gkambarb          #+#    #+#             */
/*   Updated: 2025/07/05 14:58:02 by gkambarb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*get_word(char *input, int *i)
{
	int		start;
	int		len;
	char	*str;

	start = *i;
	while (input[*i] && !is_whitespace(input[*i]) && !is_operator_char(input[*i]) && !is_quotes(input[*i]))
		(*i)++;
	len = *i - start;
	str = (char *)malloc(sizeof(char) * (len + 1));
	if (!str)
		return (NULL);
	ft_strlcpy(str, input + start, len + 1);
	return (str);
}

int	handle_word(char *input, int *i, t_token **tokens)
{
	char	*word;

	word = get_word(input, i);
	if (!word)
		return (0);
	add_token(tokens, create_token(word, T_WORD, QUOTE_NONE));
	return (1);
}
