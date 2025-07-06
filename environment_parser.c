/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment_parser.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 19:46:31 by aaycan            #+#    #+#             */
/*   Updated: 2025/07/06 20:06:29 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>
#include <stdio.h>

static t_env	*create_env_node(char *entry, t_env_error *err);

t_env	*create_env_list(char **envp)
{
	t_env		*env_list;
	t_env		*node;
	t_env_error	err;
	int			i;

	if (!envp || !*envp)
		return (NULL);
	env_list = NULL;
	i = 0;
	while (envp[i])
	{
		node = create_env_node(envp[i], &err);
		if (err == ENV_OK)
			append_env_node(&env_list, node);
		else if (err == ENV_ALLOC_ERROR)
		{
			free_env_list(env_list);
			printf("ENV_ALLOCATION_ERROR\n");
			exit(EXIT_FAILURE);
		}
		i++;
	}
	return (env_list);
}

void	free_env_list(t_env *list)
{
	t_env	*next;

	if (!list)
		return ;
	while (list)
	{
		next = list->next;
		free(list->key);
		free(list->value);
		free(list);
		list = next;
	}
}

static t_env	*create_env_node(char *entry, t_env_error *err)
{
	t_env	*node;
	char	*eq;

	eq = ft_strchr(entry, '=');
	if (!eq)
	{
		(*err) = ENV_SKIP;
		return (NULL);
	}
	node = malloc(sizeof(t_env));
	if (!node)
	{
		*err = ENV_ALLOC_ERROR;
		return (NULL);
	}
	node->key = ft_strndup(entry, eq - entry);
	node->value = ft_strdup(eq + 1);
	node->next = NULL;
	if (!node->key || !node->value)
	{
		free(node->key);
		free(node->value);
		free(node);
		*err = ENV_ALLOC_ERROR;
		return (NULL);
	}
	*err = ENV_OK;
	return (node);
}

void	append_env_node(t_env **list, t_env *new_node)
{
	t_env	*tmp;

	if (!*list)
	{
		*list = new_node;
		return ;
	}
	tmp = *list;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new_node;
}
