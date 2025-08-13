/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment_parser_pt_two.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 16:35:30 by aaycan            #+#    #+#             */
/*   Updated: 2025/08/13 15:00:50 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdlib.h>
#include <unistd.h>

static void	create_env_node_pt_two(t_env *node, t_env_error *err);
static void	create_pwd_env_node(t_env **env_list, t_env **node,
				t_env_error	*err, char *cwd);

void	create_pre_defined_env_nodes(t_env **env_list, t_env **node)
{
	t_env_error	err;
	char		*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		write(2, "Could not find current working directory\n", 41);
		exit(EXIT_FAILURE);
	}
	create_pwd_env_node(env_list, node, &err, cwd);
}

static void	create_pwd_env_node(t_env **env_list, t_env **node,
	t_env_error	*err, char *cwd)
{
	char	*pwd_val;

	pwd_val = ft_strjoin("PWD=", cwd);
	free(cwd);
	if (!pwd_val)
	{
		write(2, "ENV_ALLOCATION_ERROR\n", 21);
		exit(EXIT_FAILURE);
	}
	(*node) = create_env_node(pwd_val, err);
	free(pwd_val);
	if ((*err) == ENV_OK)
		append_env_node(env_list, (*node));
	else if ((*err) == ENV_ALLOC_ERROR)
	{
		write(2, "ENV_ALLOCATION_ERROR\n", 21);
		exit(EXIT_FAILURE);
	}
}

t_env	*create_env_node(char *entry, t_env_error *err)
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
		create_env_node_pt_two(node, err);
		return (NULL);
	}
	*err = ENV_OK;
	return (node);
}

static void	create_env_node_pt_two(t_env *node, t_env_error *err)
{
	free(node->key);
	free(node->value);
	free(node);
	*err = ENV_ALLOC_ERROR;
}
