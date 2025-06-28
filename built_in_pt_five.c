/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_pt_five.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 22:26:09 by aaycan            #+#    #+#             */
/*   Updated: 2025/06/28 22:47:19 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>

static t_env	*copy_env_list(t_env *env);
static void		sort_env_list(t_env *head);
static void		swap_env_nodes(t_env *a, t_env *b);

void	print_export_list(t_env *env)
{
	t_env	*sorted;
	t_env	*tmp;

	sorted = copy_env_list(env);
	if (!sorted)
		return ;
	sort_env_list(sorted);
	tmp = sorted;
	while (tmp)
	{
		if (tmp->key)
		{
			printf("declare -x %s", tmp->key);
			if (tmp->value)
				printf("=\"%s\"", tmp->value);
			printf("\n");
		}
		tmp = tmp->next;
	}
	free_env_list(sorted);
}

static t_env	*copy_env_list(t_env *env)
{
	t_env	*copy = NULL;
	t_env	*node;
	t_env	**last = &copy;

	while (env)
	{
		node = malloc(sizeof(t_env));
		if (!node)
			return (NULL);
		node->key = ft_strdup(env->key);
		node->value = env->value ? ft_strdup(env->value) : NULL;
		node->next = NULL;
		*last = node;
		last = &node->next;
		env = env->next;
	}
	return (copy);
}

static void	sort_env_list(t_env *head)
{
	t_env	*cur;
	int		swapped;

	if (!head)
		return;
	swapped = 1;
	while (swapped)
	{
		swapped = 0;
		cur = head;
		while (cur && cur->next)
		{
			if (ft_strcmp(cur->key, cur->next->key) > 0)
			{
				swap_env_nodes(cur, cur->next);
				swapped = 1;
			}
			cur = cur->next;
		}
	}
}

static void	swap_env_nodes(t_env *a, t_env *b)
{
	char *tmp_key = a->key;
	char *tmp_val = a->value;

	a->key = b->key;
	a->value = b->value;
	b->key = tmp_key;
	b->value = tmp_val;
}
