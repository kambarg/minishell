/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_word.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkambarb <gkambarb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 11:17:46 by gkambarb          #+#    #+#             */
/*   Updated: 2025/07/05 11:20:03 by gkambarb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*get_word(char *input, int *i)
{
	int		start;
	int		len;
	char	*str;

	start = *i;
	while (input[*i] && !is_whitespace(input[*i]) && !is_operator_char(input[*i]) && input[*i] != '\'' && input[*i] != '\"')
		(*i)++;
	len = *i - start;
	str = (char *)malloc(sizeof(char) * (len + 1));
	if (!str)
		return (NULL);
	ft_strlcpy(str, input + start, len + 1);
	return (str);
}

t_token	*create_token(char *value, int type)
{
	t_token	*token;

	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = value;
	token->type = type;
	token->quote_type = QUOTE_NONE;
	token->next = NULL;
	return (token);
}

int	handle_word(char *input, int *i, t_token **tokens)
{
	char	*word;

	word = get_word(input, i);
	if (!word)
		return (0);
	add_token(tokens, create_token(word, T_WORD));
	return (1);
}
