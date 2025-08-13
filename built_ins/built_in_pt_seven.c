/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_pt_seven.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 22:10:13 by aaycan            #+#    #+#             */
/*   Updated: 2025/08/13 15:23:06 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <unistd.h>
#include <stdlib.h>

static void	unset_command_pt_two(t_env **env_node, t_env **prev_env_node,
				t_env **env_list);
static void	unset_command_pt_three(t_env **env_node, t_env **prev_env_node);

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
	t_env	*tmp_env_node;

	(*cmd->io->exit_stat_ptr) = 0;
	if (!cmd->argv[1])
		return ;
	i = 0;
	while (cmd->argv[++i])
	{
		tmp_env_node = (*env_list);
		prev_env_node = NULL;
		while (tmp_env_node)
		{
			if (tmp_env_node->key
				&& ft_strcmp(tmp_env_node->key, cmd->argv[i]) == 0)
			{
				unset_command_pt_two(&tmp_env_node, &prev_env_node, env_list);
				break ;
			}
			else
				prev_env_node = tmp_env_node;
			tmp_env_node = tmp_env_node->next;
		}
	}
}

static void	unset_command_pt_two(t_env **env_node, t_env **prev_env_node,
	t_env **env_list)
{
	t_env	*tmp_env_node;

	tmp_env_node = NULL;
	if (!(*prev_env_node))
	{
		free((*env_node)->key);
		free((*env_node)->value);
		(*env_node)->key = NULL;
		(*env_node)->value = NULL;
		if ((*env_node)->next)
		{
			tmp_env_node = (*env_node)->next;
			free((*env_node));
			(*env_list) = tmp_env_node;
		}
		else
		{
			free(*env_node);
			(*env_list) = NULL;
		}
	}
	else
		unset_command_pt_three(env_node, prev_env_node);
}

static void	unset_command_pt_three(t_env **env_node, t_env **prev_env_node)
{
	(*prev_env_node)->next = (*env_node)->next;
	free((*env_node)->key);
	free((*env_node)->value);
	(*env_node)->key = NULL;
	(*env_node)->value = NULL;
	(*env_node)->next = NULL;
	free((*env_node));
}
