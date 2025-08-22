/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment_parser_pt_one.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 19:46:31 by aaycan            #+#    #+#             */
/*   Updated: 2025/08/22 16:03:46 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdlib.h>
#include <unistd.h>

static int		skip_pre_defined_env(char *envp, size_t *i);

t_env	*create_env_list(char **envp)
{
	t_env		*env_list;
	t_env		*node;
	t_env_error	err;
	size_t		i;

	env_list = NULL;
	create_pre_defined_env_nodes(&env_list, &node);
	i = 0;
	while (envp[i])
	{
		if (skip_pre_defined_env(envp[i], &i) == 1)
			continue ;
		node = create_env_node(envp[i], &err);
		if (err == ENV_OK)
			append_env_node(&env_list, node);
		else if (err == ENV_ALLOC_ERROR)
		{
			free_env_list(env_list);
			write(2, "ENV_ALLOCATION_ERROR\n", 21);
			exit(EXIT_FAILURE);
		}
		i++;
	}
	create_shell_level(&env_list, &node, envp);
	return (env_list);
}

static int	skip_pre_defined_env(char *envp, size_t *i)
{
	if (!(ft_strncmp(envp, "SHLVL", 5))
		|| !(ft_strncmp(envp, "PWD", 3)))
	{
		(*i)++;
		return (1);
	}
	return (0);
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
