/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_pt_four.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 21:58:23 by aaycan            #+#    #+#             */
/*   Updated: 2025/06/28 22:21:55 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>

static void		split_key_value(char *arg, char **key, char **value);
static t_env	*find_env_node(t_env *env, const char *key);
static int		is_valid_identifier(const char *s);

void	export_command(t_command *cmd, t_env **env_list)
{
	char	*key;
	char	*value;
	t_env	*existing;
	t_env	*new_node;
	int		i;

	i = 1;
	if (!cmd->argv[1])
	{
    	print_export_list(*env_list);
    	return;
	}
	while (cmd->argv[i])
	{
		split_key_value(cmd->argv[i], &key, &value);
		if (!is_valid_identifier(cmd->argv[i]))
		{
			printf("export: `%s`: not a valid identifier\n", cmd->argv[i]);
			free(key);
			free(value);
			i++;
			continue;
		}
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
			new_node = malloc(sizeof(t_env));
			if (!new_node)
			{
				free(key);
				free(value);
				return ;
			}
			new_node->key = key;
			new_node->value = value;
			new_node->next = NULL;
			append_env_node(env_list, new_node);
		}
		i++;
	}
}

static void	split_key_value(char *arg, char **key, char **value)
{
	char	*eq;

	eq = ft_strchr(arg, '=');
	if (!eq)
	{
		*key = NULL;
		*value = NULL;
		return ;
	}
	*key = ft_strndup(arg, eq - arg);
	*value = ft_strdup(eq + 1);
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
	if (!ft_isalpha(s[0]) && s[0] != '_')
		return (0);
	i = 1;
	while (s[i] && s[i] != '=')
	{
		if (!ft_isalnum(s[i]) && s[i] != '_')
			return (0);
		i++;
	}
	return (1);
}
