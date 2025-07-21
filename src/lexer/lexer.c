/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: worodhazem <worodhazem@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 10:15:12 by gkambarb          #+#    #+#             */
/*   Updated: 2025/07/21 01:01:23 by worodhazem       ###   ########.fr       */
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

void print_tokens(t_token *tokens) {
    int i = 0;
    while (tokens) {
        printf("Token %d: '%s'\n", i, tokens->value ? tokens->value : "(null)");
        tokens = tokens->next;
        i++;
    }
}


t_token *lexer(char *input)
{
    int i = 0;
    t_token *tokens = NULL;
    char *arg_buf = NULL;
    char *tmp = NULL;

    while (input[i])
    {
        // 1. Skip whitespace and push current argument if any
        if (is_whitespace(input[i]))
        {
            if (arg_buf && arg_buf[0])
            {
                add_token(&tokens, create_token(arg_buf, T_WORD, QUOTE_NONE));
                free(arg_buf);
                arg_buf = NULL;
            }
            i++;
        }
        // 2. Operator? Push arg buffer if any, then handle operator
        else if (is_operator_char(input[i]))
        {
            if (arg_buf && arg_buf[0])
            {
                add_token(&tokens, create_token(arg_buf, T_WORD, QUOTE_NONE));
                free(arg_buf);
                arg_buf = NULL;
            }
            if (!handle_operator(input, &i, &tokens))
                return (free_tokens(tokens), NULL);
            i++;
        }
        // 3. Quoted string? Grab quoted and append to arg buffer
        else if (is_quotes(input[i]))
        {
            char *quoted = get_quoted_str(input, &i, input[i]);
            if (!quoted)
            {
                free(arg_buf);
                return (free_tokens(tokens), NULL);
            }
            if (!arg_buf) arg_buf = ft_strdup("");
            tmp = ft_strjoin(arg_buf, quoted);
            free(arg_buf);
            arg_buf = tmp;
            free(quoted);
        }
        // 4. Unquoted word? Grab and append to arg buffer
        else
        {
            int start = i;
            while (input[i] && !is_whitespace(input[i]) && !is_operator_char(input[i]) && !is_quotes(input[i]))
                i++;
            int len = i - start;
            char *word = (char *)malloc(len + 1);
            if (!word)
            {
                free(arg_buf);
                return (free_tokens(tokens), NULL);
            }
            ft_strlcpy(word, input + start, len + 1);
            if (!arg_buf) arg_buf = ft_strdup("");
            tmp = ft_strjoin(arg_buf, word);
            free(arg_buf);
            arg_buf = tmp;
            free(word);
        }
    }
    // Add any final argument after loop
    if (arg_buf && arg_buf[0])
        add_token(&tokens, create_token(arg_buf, T_WORD, QUOTE_NONE));
    free(arg_buf);
    return tokens;
}
