/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_pt_two.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 03:23:46 by aaycan            #+#    #+#             */
/*   Updated: 2025/06/28 21:37:23 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "stdlib.h"

char	*ft_strchr(const char *s, int c)
{
	size_t			i;
	char			*typecasts;
	unsigned char	tc_c;

	typecasts = (char *)s;
	tc_c = (unsigned char)c;
	i = 0;
	while (typecasts[i])
	{
		if (typecasts[i] == tc_c)
			return (&typecasts[i]);
		i++;
	}
	if (tc_c == 0)
		return ((char *)&typecasts[ft_strlen((const char *)(typecasts))]);
	else
		return (NULL);
}

char	*ft_strdup(const char *s1)
{
	size_t	sizeofs1;
	char	*copy;
	size_t	i;

	i = 0;
	sizeofs1 = ft_strlen(s1);
	copy = (char *)malloc(sizeofs1 + 1);
	if (!copy)
		return (NULL);
	while (s1[i])
	{
		copy[i] = s1[i];
		i++;
	}
	copy[i] = '\0';
	return (copy);
}

int	ft_isalnum(int v)
{
	if ((v >= 'a' && v <= 'z') || (v >= 'A' && v <= 'Z')
		|| (v >= '0' && v <= '9'))
		return (1);
	return (0);
}

int	is_env_char(char c)
{
	if (ft_isalnum(c) || (c == '_'))
		return (1);
	return (0);
}

int	ft_isdigit(int v)
{
	if (v >= '0' && v <= '9')
		return (1);
	return (0);
}
