/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_pt_seven.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 22:10:13 by aaycan            #+#    #+#             */
/*   Updated: 2025/07/06 22:10:29 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>

void	swap_env_nodes(t_env *a, t_env *b)
{
	char *tmp_key;
	char *tmp_val;

	tmp_key = a->key;
	tmp_val = a->value;
	a->key = b->key;
	a->value = b->value;
	b->key = tmp_key;
	b->value = tmp_val;
}

void	export_malloc_fail_handler(t_env *copy, t_env *node, int option)
{
	if (option == 1)
		free_env_list(copy);
	if (option == 2)
	{
		free(node);
		free_env_list(copy);
	}
	if (option == 3)
	{
		free(node->key);
		free(node);
		free_env_list(copy);
	}
	printf("ALLOCATION_ERROR for Export Command\n");
	
}
