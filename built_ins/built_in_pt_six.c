/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_pt_six.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 20:33:03 by aaycan            #+#    #+#             */
/*   Updated: 2025/07/22 17:21:52 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdlib.h>

static t_env	*copy_env_list(t_env *env);
static int		copy_env_list_pt_two(t_env *env, t_env *node, t_env *copy);
static void		sort_env_list(t_env *head);

void	print_export_list(t_command *cmd, t_env *env)
{
	t_env	*sorted;
	t_env	*tmp;

	sorted = copy_env_list(env);
	if (!sorted)
		return ;
	sort_env_list(sorted);
	tmp = sorted;
	if (cmd->outfile)
		print_export_list_to_outfile(cmd, tmp);
	else
		print_export_list_to_stdout(tmp);
	free_env_list(sorted);
}

static t_env	*copy_env_list(t_env *env)
{
	t_env	*copy;
	t_env	*node;
	t_env	**last;

	copy = NULL;
	node = NULL;
	last = &copy;
	while (env)
	{
		node = malloc(sizeof(t_env));
		if (!node)
		{
			export_malloc_fail_handler(copy, env, 1);
			return (NULL);
		}
		if (copy_env_list_pt_two(env, node, copy) != 0)
			return (NULL);
		node->next = NULL;
		*last = node;
		last = &node->next;
		env = env->next;
	}
	return (copy);
}

static int	copy_env_list_pt_two(t_env *env, t_env *node, t_env *copy)
{
	node->key = ft_strdup(env->key);
	if (!node->key)
	{
		export_malloc_fail_handler(copy, env, 2);
		return (1);
	}
	if (env->value)
	{
		node->value = ft_strdup(env->value);
		if (!node->value)
		{
			export_malloc_fail_handler(copy, node, 3);
			return (1);
		}
	}
	else
		node->value = NULL;
	return (0);
}

static void	sort_env_list(t_env *head)
{
	t_env	*cur;
	int		swapped;

	if (!head)
		return ;
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
