/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_pt_five.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 02:38:08 by aaycan            #+#    #+#             */
/*   Updated: 2025/07/13 00:10:44 by aaycan           ###   ########.fr       */
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

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*result;
	int		i;
	int		j;
	size_t	total_len;

	if (!s1 || !s2)
		return (NULL);
	i = 0;
	total_len = ft_strlen(s1) + ft_strlen(s2);
	result = (char *)malloc((total_len * sizeof(char)) + 1);
	if (result == NULL)
		return (NULL);
	while (s1[i] != '\0')
	{
		result[i] = ((unsigned char *)s1)[i];
		i++;
	}
	j = 0;
	while (s2[j] != '\0')
		result[i++] = ((unsigned char *)s2)[j++];
	result[i] = '\0';
	return (result);
}
