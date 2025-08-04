/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_collector.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 03:26:18 by aaycan            #+#    #+#             */
/*   Updated: 2025/08/04 18:45:02 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdlib.h>
#include <unistd.h>

static int	count_dollar_signs(char *line);

t_gc	*init_garbage_collector(char *line)
{
	t_gc	*gc;

	gc = malloc(sizeof(t_gc));
	if (!gc)
	{
		write(2, "Garbage collector initialization failed\n", 40);
		return (NULL);
	}
	gc->ptrs = malloc(sizeof(void *) * (1024 + (count_dollar_signs(line))
				+ (10 * count_tokens(line))));
	if (!gc->ptrs)
	{
		write(2, "Garbage collector initialization failed\n", 40);
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

static int	count_dollar_signs(char *line)
{
	int	i;
	int	sign_count;

	i = 0;
	sign_count = 0;
	while (line[i])
	{
		if (line[i] == '$')
			sign_count++;
		i++;
	}
	return (sign_count);
}
