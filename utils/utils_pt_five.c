/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_pt_five.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 02:38:08 by aaycan            #+#    #+#             */
/*   Updated: 2025/07/12 20:06:42 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdlib.h>

void	*ft_calloc(size_t count, size_t size)
{
	unsigned char	*tmp;
	size_t			i;

	i = 0;
	tmp = malloc(size * count);
	if (!tmp)
		return (NULL);
	while (i < size * count)
	{
		tmp[i] = 0;
		i++;
	}
	return (tmp);
}

char	*get_minishell_env(char *key, t_env *env_list)
{
	while (env_list)
	{
		if (env_list->key && ft_strcmp(env_list->key, key) == 0)
			return (env_list->value);
		env_list = env_list->next;
	}
	return (NULL);
}

void	free_string_array(char **str_arr)
{
	int	i;

	i = -1;
	while (str_arr[++i])
		free(str_arr[i]);
	free(str_arr);
}

int	search_str_in_str(const char *haystack, const char *needle)
{
	size_t	i;
	size_t	j;

	if (!haystack && !needle)
		return (0);
	i = 0;
	while (haystack[i])
	{
		j = 0;
		while (haystack[i + j] == needle[j])
		{
			if ((haystack[i + j] == '\0') && (needle[j] == '\0'))
				return (i + 1);
			j++;
		}
		if (needle[j] == '\0')
			return (i + 1);
		i++;
	}
	return (0);
}
