/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkambarb <gkambarb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 12:50:38 by gkambarb          #+#    #+#             */
/*   Updated: 2024/07/18 11:55:54 by gkambarb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	*ft_fcontent(t_list *lst, void *(*f)(void *))
{
	void	*tst;

	tst = f(lst->content);
	if (!tst)
		return (NULL);
	return (tst);
}

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*new;
	t_list	*tmp;
	void	*fcontent;

	if (lst == NULL)
		return (NULL);
	new = 0;
	while (lst != NULL)
	{
		fcontent = ft_fcontent(lst, f);
		if (fcontent == NULL)
			ft_lstclear(&new, del);
		tmp = ft_lstnew(fcontent);
		if (tmp == NULL)
		{
			del(fcontent);
			ft_lstclear(&new, del);
			return (NULL);
		}
		ft_lstadd_back(&new, tmp);
		lst = lst->next;
	}
	return (new);
}
