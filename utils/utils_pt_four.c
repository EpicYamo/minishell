/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_pt_four.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 02:36:18 by aaycan            #+#    #+#             */
/*   Updated: 2025/07/09 19:13:22 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdlib.h>

static int	countofwords(const char *str, char c);
static char	*set_word(const char *str, int start, int end);
static void	*fail_free(char **strs, int count);
static int	set_vars(size_t *i, int *j, int *pos_word, char const *s);

char	**ft_split(char const *s, char c)
{
	char	**dim;
	size_t	i;
	int		j;
	int		pos_word;

	if (set_vars(&i, &j, &pos_word, s) == 0)
		return (NULL);
	dim = ft_calloc((countofwords(s, c) + 1), sizeof(char *));
	if (!dim)
		return (NULL);
	while (i <= ft_strlen(s))
	{
		if (s[i] != c && pos_word < 0)
			pos_word = i;
		else if (((s[i]) == c || i == ft_strlen(s)) && pos_word >= 0)
		{
			dim[j] = set_word(s, pos_word, i);
			if (!(dim[j]))
				return (fail_free(dim, j));
			pos_word = -1;
			j++;
		}
		i++;
	}
	return (dim);
}

static int	set_vars(size_t *i, int *j, int *pos_word, char const *s)
{
	*i = 0;
	*j = 0;
	*pos_word = -1;
	if (!s)
		return (0);
	return (1);
}

static int	countofwords(const char *str, char c)
{
	int	count;
	int	x;

	count = 0;
	x = 0;
	while (*str)
	{
		if (*str != c && x == 0)
		{
			x = 1;
			count++;
		}
		else if (*str == c)
			x = 0;
		str++;
	}
	return (count);
}

static char	*set_word(const char *str, int first, int last)
{
	char	*word;
	int		i;

	i = 0;
	word = malloc((last - first + 1) * sizeof(char));
	if (!word)
		return (NULL);
	while (first < last)
	{
		word[i] = str[first];
		i++;
		first++;
	}
	word[i] = 0;
	return (word);
}

static void	*fail_free(char **interdim, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		free(interdim[i]);
		i++;
	}
	free(interdim);
	return (NULL);
}
