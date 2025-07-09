/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_collector.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 03:26:18 by aaycan            #+#    #+#             */
/*   Updated: 2025/07/09 19:13:13 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdlib.h>
#include <stdio.h>

t_gc	*init_garbage_collector(char *line)
{
	t_gc	*gc;

	gc = malloc(sizeof(t_gc));
	if (!gc)
	{
		printf("\033[0;31mGarbage collector initialization failed\033[0m\n");
		return (NULL);
	}
	gc->ptrs = malloc(sizeof(void *) * (1024 + (4 * count_tokens(line))));
	if (!gc->ptrs)
	{
		printf("\033[0;31mGarbage collector initialization failed\033[0m\n");
		free(gc);
		return (NULL);
	}
	gc->g_index = 0;
	return (gc);
}

void	*gc_malloc(t_gc *gc, size_t size)
{
	void	*ptr;

	ptr = malloc(size);
	if (!ptr)
		return (NULL);
	gc->ptrs[gc->g_index++] = ptr;
	return (ptr);
}

void	gc_add(t_gc *gc, void *ptr)
{
	gc->ptrs[gc->g_index++] = ptr;
}

void	gc_collect_all(t_gc *gc)
{
	size_t	i;

	if (!gc)
		return ;
	i = gc->g_index;
	while (i > 0)
	{
		i--;
		free(gc->ptrs[i]);
	}
	free(gc->ptrs);
	free(gc);
}
