/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_quoted_string.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkambarb <gkambarb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 11:10:05 by gkambarb          #+#    #+#             */
/*   Updated: 2025/07/05 14:43:59 by gkambarb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*get_quoted_str(char *input, int *i, char quote)
{
	int		start;
	int		len;
	char	*str;

	(*i)++;
	start = *i;
	while (input[*i] && input[*i] != quote)
		(*i)++;
	if (!input[*i])
	{
		print_error(NULL, "syntax error: unclosed quotes");
		return (NULL);
	}
	len = *i - start;
	str = (char *)malloc(sizeof(char) * (len + 1));
	if (!str)
		return (NULL);
	ft_strlcpy(str, input + start, len + 1);
	(*i)++;
	return (str);
}
// t_token	*create_token_with_quote(char *value, int type, int quote_type)
// {
// 	t_token	*token;

// 	token = (t_token *)malloc(sizeof(t_token));
// 	if (!token)
// 		return (NULL);
// 	token->value = value;
// 	token->type = type;
// 	token->quote_type = quote_type;
// 	token->next = NULL;
// 	return (token);
// }

int	handle_quoted_string(char *input, int *i, t_token **tokens)
{
	char	*word;
	int		quote_type;

	if (input[*i] == '\'')
		quote_type = QUOTE_SINGLE;
	else
		quote_type = QUOTE_DOUBLE;
	word = get_quoted_str(input, i, input[*i]);
	if (!word)
		return (0);
	add_token(tokens, create_token(word, T_WORD, quote_type));
	return (1);
}
