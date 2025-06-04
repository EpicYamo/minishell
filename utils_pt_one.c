/* ************************************************************************************** */
/*                                                                                        */
/*                                                                   :::      ::::::::    */
/*   utils_pt_one.c                                                :+:      :+:    :+:    */
/*                                                               +:+ +:+         +:+      */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr>              +#+  +:+       +#+         */
/*                                                           +#+#+#+#+#+   +#+            */
/*   Created: 2025/06/04 15:29:26 by aaycan                       #+#    #+#              */
/*   Updated: 2025/06/04 15:29:26 by aaycan                      ###   ########.tr        */
/*                                                                                        */
/* ************************************************************************************** */

#include <stdlib.h>

int	is_metachar(char c)
{
	return ((c == '|') || (c == '<') || (c == '>'));
}

int	ft_isspace(char c)
{
	return ((c == ' ') || (c >= 9 && c <= 13));
}

char	*ft_strndup(const char *s, size_t n)
{
	char *dup;
	size_t i;

	if (!s)
		return (NULL);
	dup = malloc(n + 1);
	if (!dup)
		return (NULL);
	i = 0;
	while (i < n)
	{
		dup[i] = s[i];
		i++;
	}
	dup[n] = '\0';
	return (dup);
}

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}
