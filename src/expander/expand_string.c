/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_string.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wuabdull <wuabdull@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 14:16:40 by gkambarb          #+#    #+#             */
/*   Updated: 2025/07/16 15:08:35 by wuabdull         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*ft_strjoin_char(char *s, char c)
{
	char	*result;
	int		len;
	int		i;

	if (!s)
		return (NULL);
	len = ft_strlen(s);
	result = (char *)malloc(sizeof(char) * (len + 2));
	if (!result)
		return (NULL);
	i = 0;
	while (s[i])
	{
		result[i] = s[i];
		i++;
	}
	result[i] = c;
	result[i + 1] = '\0';
	return (result);
}

static char	*handle_dollar_char(char *str, t_shell *shell, int *i, char *result)
{
	char	*var;
	char	*new_result;

	var = expand_var(str, shell, i);
	if (!var)
		return (free(result), NULL);
	new_result = ft_strjoin(result, var);
	free(var);
	if (!new_result)
		return (free(result), NULL);
	free(result);
	return (new_result);
}

static char	*handle_regular_char(char *result, char c)
{
	char	*new_result;

	new_result = ft_strjoin_char(result, c);
	if (!new_result)
		return (free(result), NULL);
	free(result);
	return (new_result);
}

// Note: single quotes no expansion at all
char	*expand_quoted_string(char *str, t_shell *shell,
	int quote_type)
{
	int		i;
	char	*result;

	if (!str || !shell)
		return (NULL);
	if (quote_type == QUOTE_SINGLE)
		return (ft_strdup(str));
	i = 0;
	result = ft_strdup("");
	if (!result)
		return (NULL);
	while (str[i])
	{
		if (str[i] == '$' && (quote_type == QUOTE_DOUBLE
				|| quote_type == QUOTE_NONE))
			result = handle_dollar_char(str, shell, &i, result);
		else
			result = handle_regular_char(result, str[i++]);
		if (!result)
			return (NULL);
	}
	return (result);
}
