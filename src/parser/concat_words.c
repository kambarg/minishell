/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   concat_words.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkambarb <gkambarb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 11:10:35 by gkambarb          #+#    #+#             */
/*   Updated: 2025/07/24 11:19:24 by gkambarb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	init_concat_ctx(t_concat_ctx *ctx, t_token **tokens)
{
	ctx->combined_value = ft_strdup((*tokens)->value);
	if (!ctx->combined_value)
		return (0);
	ctx->final_quote_type = (*tokens)->quote_type;
	ctx->has_unquoted = ((*tokens)->quote_type == QUOTE_NONE);
	ctx->token_count = 1;
	ctx->current_token = (*tokens)->next;
	*tokens = (*tokens)->next;
	return (1);
}

static void	process_consecutive_words(t_token **tokens, t_concat_ctx *ctx)
{
	char	*temp;

	while (ctx->current_token && ctx->current_token->type == T_WORD
		&& ctx->current_token->pre_space == 0)
	{
		temp = ft_strjoin(ctx->combined_value, ctx->current_token->value);
		free(ctx->combined_value);
		if (!temp)
			break ;
		ctx->combined_value = temp;
		if (ctx->current_token->quote_type == QUOTE_NONE)
			ctx->has_unquoted = 1;
		else if (ctx->final_quote_type == QUOTE_NONE)
			ctx->final_quote_type = ctx->current_token->quote_type;
		ctx->token_count++;
		ctx->current_token = ctx->current_token->next;
		*tokens = ctx->current_token;
	}
}

static void	adjust_final_quote_type(t_concat_ctx *ctx)
{
	if (ctx->token_count > 1 && ctx->has_unquoted)
		ctx->final_quote_type = QUOTE_NONE;
}

int	concat_words(t_token **tokens, t_command *cur)
{
	t_concat_ctx	ctx;

	if (!*tokens || (*tokens)->type != T_WORD)
		return (0);
	if (!init_concat_ctx(&ctx, tokens))
		return (0);
	process_consecutive_words(tokens, &ctx);
	adjust_final_quote_type(&ctx);
	if (!add_argument(cur, ctx.combined_value, ctx.final_quote_type))
		return (free(ctx.combined_value), 0);
	free(ctx.combined_value);
	return (1);
}
