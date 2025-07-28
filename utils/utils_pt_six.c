/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_pt_six.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 22:03:12 by aaycan            #+#    #+#             */
/*   Updated: 2025/07/28 22:25:41 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <unistd.h>
#include <stdlib.h>

int	env_list_size(t_env *env)
{
	int	count;

	count = 0;
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
	int		i;
	int		size;
	char	**envp;
	char	*tmp;

	i = 0;
	size = env_list_size(env_list);
	envp = malloc(sizeof(char *) * (size + 1));
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

char	*resolve_path(const char *cmd, t_env *env_list)
{
	char	**paths;
	char	*full_path;
	char	*tmp;
	int		i;

	if (access(cmd, X_OK) == 0)
		return (ft_strdup(cmd));
	char *path_value = get_minishell_env("PATH", env_list);
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
			free_string_array(paths);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	free_string_array(paths);
	return (NULL);
}
