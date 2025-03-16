/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkambarb <gkambarb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 12:49:49 by gkambarb          #+#    #+#             */
/*   Updated: 2024/07/15 15:20:04 by gkambarb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	if ((*lst) == NULL)
		*lst = new;
	else if ((*lst)->next == NULL)
	{
		(*lst)->next = new;
		new->next = NULL;
	}
	else
	{
		(ft_lstlast(*lst))->next = new;
	}
}
