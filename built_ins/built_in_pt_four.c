/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_pt_four.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 21:58:23 by aaycan            #+#    #+#             */
/*   Updated: 2025/08/04 18:06:51 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdio.h>
#include <stdlib.h>

static int		export_command_pt_two(t_env **env_list, char *key, char *value);
static int		export_command_pt_three(t_env **env_list,
					char *key, char *value);
static t_env	*find_env_node(t_env *env, const char *key);
static int		is_valid_identifier(const char *s);

void	export_command(t_command *cmd, t_env **env_list)
{
	char	*key;
	char	*value;
	int		i;

	if (!cmd->argv[1])
	{
		print_export_list(cmd, *env_list);
		return ;
	}
	i = 1;
	while (cmd->argv[i])
	{
		if (!is_valid_identifier(cmd->argv[i]))
		{
			printf("export: `%s`: not a valid identifier\n", cmd->argv[i]);
			(*cmd->io->exit_stat_ptr) = 1;
			i++;
			continue ;
		}
		if (split_key_value(cmd->argv[i], &key, &value) != 0)
			continue ;
		if (export_command_pt_two(env_list, key, value) != 0)
			continue ;
		i++;
	}
}

static int	export_command_pt_two(t_env **env_list, char *key, char *value)
{
	t_env	*existing;

	existing = find_env_node(*env_list, key);
	if (existing)
	{
		if (value)
		{
			free(existing->value);
			existing->value = value;
		}
		else if (!existing->value)
			existing->value = NULL;
		free(key);
	}
	else
	{
		if (export_command_pt_three(env_list, key, value) != 0)
			return (1);
	}
	return (0);
}

static int	export_command_pt_three(t_env **env_list, char *key, char *value)
{
	t_env	*new_node;

	new_node = malloc(sizeof(t_env));
	if (!new_node)
	{
		free(key);
		free(value);
		printf("ALLOCATION_ERROR for Export Command\n");
		return (1);
	}
	new_node->key = key;
	new_node->value = value;
	new_node->next = NULL;
	append_env_node(env_list, new_node);
	return (0);
}

static t_env	*find_env_node(t_env *env, const char *key)
{
	while (env)
	{
		if (env->key && ft_strcmp(env->key, key) == 0)
			return (env);
		env = env->next;
	}
	return (NULL);
}

static int	is_valid_identifier(const char *s)
{
	int	i;

	if (!s || !s[0])
		return (0);
	if (!ft_isalpha(s[0]) && (s[0] != '_'))
		return (0);
	i = 1;
	while (s[i] && (s[i] != '='))
	{
		if (!ft_isalnum(s[i]) && (s[i] != '_'))
			return (0);
		i++;
	}
	return (1);
}
