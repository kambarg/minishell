/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstlast.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkambarb <gkambarb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 12:50:30 by gkambarb          #+#    #+#             */
/*   Updated: 2024/07/18 11:56:32 by gkambarb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstlast(t_list *lst)
{
	t_list	*tmp;
	t_list	*ret;

	if (lst == NULL)
		return (0);
	ret = lst;
	tmp = lst->next;
	while (tmp != NULL)
	{
		ret = tmp;
		tmp = tmp->next;
	}
	return (ret);
}
