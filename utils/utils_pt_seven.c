/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_pt_seven.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 15:57:53 by aaycan            #+#    #+#             */
/*   Updated: 2025/08/04 20:11:31 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

static char	*try_to_access_path(const char *cmd, char **paths);
static int	create_path_without_cmd(char **tmp, char **paths, int i);
static int	create_full_path(char **full_path, char *tmp, char **paths,
				const char *cmd);

char	*resolve_path(const char *cmd, t_env *env_list)
{
	char	**paths;
	char	*path_value;
	char	*full_path;

	if (access(cmd, X_OK) == 0)
	{
		full_path = ft_strdup(cmd);
		return (full_path);
	}
	path_value = get_minishell_env("PATH", env_list);
	if (!path_value)
		return (NULL);
	paths = ft_split(path_value, ':');
	if (!paths)
		return (NULL);
	full_path = try_to_access_path(cmd, paths);
	return (full_path);
}

static char	*try_to_access_path(const char *cmd, char **paths)
{
	int		i;
	char	*full_path;
	char	*tmp;

	i = 0;
	while (paths[i])
	{
		if (create_path_without_cmd(&tmp, paths, i) != 0)
			return (NULL);
		if (create_full_path(&full_path, tmp, paths, cmd) != 0)
			return (NULL);
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

static int	create_path_without_cmd(char **tmp, char **paths, int i)
{
	(*tmp) = ft_strjoin(paths[i], "/");
	if (!(*tmp))
	{
		free_string_array(paths);
		perror("malloc");
		return (1);
	}
	return (0);
}

static int	create_full_path(char **full_path, char *tmp, char **paths,
	const char *cmd)
{
	(*full_path) = ft_strjoin(tmp, cmd);
	if (!(*full_path))
	{
		free(tmp);
		free_string_array(paths);
		perror("malloc");
		return (1);
	}
	free(tmp);
	return (0);
}
