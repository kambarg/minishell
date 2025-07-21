/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_functions.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wuabdull <wuabdull@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 20:24:21 by wuabdull          #+#    #+#             */
/*   Updated: 2025/07/21 20:24:30 by wuabdull         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

int	is_operator_char(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

int	is_quotes(char c)
{
	return (c == '\'' || c == '\"');
}

void	print_tokens(t_token *tokens)
{
	int	i;

	i = 0;
	while (tokens != NULL)
	{
		printf("Token %d: '", i);
		if (tokens->value != NULL)
			printf("%s", tokens->value);
		else
			printf("(null)");
		printf("'\n");
		tokens = tokens->next;
		i++;
	}
}
