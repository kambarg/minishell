/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkambarb <gkambarb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 12:51:03 by gkambarb          #+#    #+#             */
/*   Updated: 2024/07/18 11:55:20 by gkambarb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_lstsize(t_list *lst)
{
	int		n;
	t_list	*tmp;

	if (lst == NULL)
		return (0);
	tmp = lst->next;
	n = 1;
	while (tmp != NULL)
	{
		tmp = tmp->next;
		n++;
	}
	return (n);
}
