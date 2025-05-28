/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abessa-m <abessa-m@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 13:16:20 by abessa-m          #+#    #+#             */
/*   Updated: 2025/05/14 10:39:21 by abessa-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//	malloc(nmemb * size);

//void	*malloc(size_t size);
//void	ft_putstr_fd(char *s, int fd);
void	*ft_malloc(int total_size)
{
	void	*ptr;

	if (total_size == 0)
		return (NULL);
	ptr = (void *) malloc(total_size);
	if (ptr == NULL)
	{
		ft_putstr_fd("ERROR malloc failed to alloc memory!", 2);
		exit(1);
	}
	return (ptr);
}
