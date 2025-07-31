/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_pt_six.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 22:03:12 by aaycan            #+#    #+#             */
/*   Updated: 2025/07/31 16:33:51 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdlib.h>

static int	add_var_to_envp(t_env *env_list, char **envp, int *i);

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

	i = 0;
	size = env_list_size(env_list);
	envp = malloc(sizeof(char *) * (size + 1));
	if (!envp)
		return (NULL);
	while (env_list)
	{
		if (env_list->key && env_list->value)
		{
			if (add_var_to_envp(env_list, envp, &i) != 0)
				return (NULL);
		}
		env_list = env_list->next;
	}
	envp[i] = NULL;
	return (envp);
}

static int	add_var_to_envp(t_env *env_list, char **envp, int *i)
{
	char	*tmp;

	tmp = ft_strjoin(env_list->key, "=");
	if (!tmp)
	{
		envp[(*i)] = NULL;
		free_string_array(envp);
		return (1);
	}
	envp[(*i)] = ft_strjoin(tmp, env_list->value);
	if (!envp[(*i)])
	{
		envp[(*i)] = NULL;
		free_string_array(envp);
		free(tmp);
		return (1);
	}
	free(tmp);
	(*i)++;
	return (0);
}
