/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_pt_seven.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 22:10:13 by aaycan            #+#    #+#             */
/*   Updated: 2025/08/04 19:10:08 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <unistd.h>
#include <stdlib.h>

static void	unset_command_pt_two(t_env **env_list, t_env **prev_env_node);
static void	unset_command_pt_three(t_env **env_list, t_env **prev_env_node);

void	swap_env_nodes(t_env *a, t_env *b)
{
	char	*tmp_key;
	char	*tmp_val;

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
	write(2, "ALLOCATION_ERROR for Export Command\n", 36);
}

void	unset_command(t_command *cmd, t_env **env_list)
{
	int		i;
	t_env	*prev_env_node;
	t_env	*head_env_node;

	if (!cmd->argv[1])
		return ;
	i = 0;
	prev_env_node = NULL;
	head_env_node = (*env_list);
	while (cmd->argv[++i])
	{
		(*env_list) = head_env_node;
		while (*(env_list))
		{
			if ((*env_list)->key
				&& ft_strcmp((*env_list)->key, cmd->argv[i]) == 0)
			{
				unset_command_pt_two(env_list, &prev_env_node);
				break ;
			}
			else
				prev_env_node = (*env_list);
			(*env_list) = (*env_list)->next;
		}
	}
}

static void	unset_command_pt_two(t_env **env_list, t_env **prev_env_node)
{
	t_env	*tmp_env_node;

	tmp_env_node = NULL;
	if (!(*prev_env_node))
	{
		(*env_list)->key = NULL;
		(*env_list)->value = NULL;
		free((*env_list)->key);
		free((*env_list)->value);
		if ((*env_list)->next)
		{
			tmp_env_node = (*env_list)->next;
			free((*env_list));
			(*env_list) = tmp_env_node;
		}
		else
			free((*env_list));
	}
	else
		unset_command_pt_three(env_list, prev_env_node);
}

static void	unset_command_pt_three(t_env **env_list, t_env **prev_env_node)
{
	(*prev_env_node)->next = (*env_list)->next;
	(*env_list)->key = NULL;
	(*env_list)->value = NULL;
	(*env_list)->next = NULL;
	free((*env_list)->key);
	free((*env_list)->value);
	free((*env_list));
}
