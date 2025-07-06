/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_pt_five.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 22:26:09 by aaycan            #+#    #+#             */
/*   Updated: 2025/07/06 22:12:03 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>

static int	split_key_value_pt_two(char *arg, char **key, char **value);

int	split_key_value(char *arg, char **key, char **value)
{
	char	*eq;

	eq = ft_strchr(arg, '=');
	if (eq)
	{
		*key = ft_strndup(arg, eq - arg);
		if (!(*key))
		{
			printf("ALLOCATION_ERROR for Export Command\n");
			return (1);
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
		{
			
			printf("ALLOCATION ERROR for Export Command\n");
			return (NULL);
		}
		node->key = ft_strdup(env->key);
		if (!node->key)
		{

			printf("ALLOCATION ERROR for Export Command\n");
			return (NULL);
		}
		if (env->value)
		{
			free(key);
			printf("ALLOCATION_ERROR for Export Command\n");
			return (1);
		}
	}
	else
	{
		if (split_key_value_pt_two(arg, key, value) != 0)
			return (1);
	}
	return (0);
}

static int	split_key_value_pt_two(char *arg, char **key, char **value)
{
	*value = NULL;
	*key = ft_strdup(arg);
	if (!(*key))
	{
		printf("ALLOCATION_ERROR for Export Command\n");
		return (1);
	}
	return (0);
}
