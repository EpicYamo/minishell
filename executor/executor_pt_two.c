/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_pt_two.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 18:04:26 by aaycan            #+#    #+#             */
/*   Updated: 2025/07/22 18:05:07 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/wait.h>
#include <fcntl.h>

int	env_list_size(t_env *env)
{
	int	count = 0;

	while (env)
	{
		if (env->key && env->value)
			count++;
		env = env->next;
	}
	return (count);
}

char	**get_envp(t_env *env_list)
{
	int		i = 0;
	int		size = env_list_size(env_list);
	char	**envp = malloc(sizeof(char *) * (size + 1));
	char	*tmp;

	if (!envp)
		return (NULL);
	while (env_list)
	{
		if (env_list->key && env_list->value)
		{
			tmp = ft_strjoin(env_list->key, "=");
			envp[i] = ft_strjoin(tmp, env_list->value);
			free(tmp);
			i++;
		}
		env_list = env_list->next;
	}
	envp[i] = NULL;
	return (envp);
}

void	ft_free_split(char **split)
{
	int	i = 0;

	if (!split)
		return ;
	while (split[i])
		free(split[i++]);
	free(split);
}

char	*get_env_value(t_env *env_list, const char *key)
{
	while (env_list)
	{
		if (env_list->key && strcmp(env_list->key, key) == 0)
			return (env_list->value);
		env_list = env_list->next;
	}
	return (NULL);
}

char	*resolve_path(const char *cmd, t_env *env_list)
{
	char	**paths;
	char	*full_path;
	char	*tmp;
	int		i;

	if (access(cmd, X_OK) == 0)
		return (ft_strdup(cmd));
	char *path_value = get_env_value(env_list, "PATH");
	if (!path_value)
		return (NULL);
	paths = ft_split(path_value, ':');
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		tmp = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin(tmp, cmd);
		free(tmp);
		if (access(full_path, X_OK) == 0)
		{
			ft_free_split(paths);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	ft_free_split(paths);
	return (NULL);
}
