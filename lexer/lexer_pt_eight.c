/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_pt_eight.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 14:52:11 by aaycan            #+#    #+#             */
/*   Updated: 2025/08/09 19:39:09 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdlib.h>

static int	init_vars_strip_quotes(char **result, size_t *i, size_t *j,
				char *s);

int	strip_quotes_and_apply_token(t_gc *gc, char **token,
	t_interpret interpret_data, size_t i)
{
	(*token) = strip_quotes(*token, interpret_data, i);
	if (!(*token))
		return (1);
	gc_add(gc, (*token));
	return (0);
}

int	init_interpret_set(t_gc *gc, t_interpret *interpret_set, char *line)
{
	size_t	i;

	(*interpret_set).token_count = count_tokens(line);
	(*interpret_set).flag_set = gc_malloc(gc,
			(sizeof(size_t) * (*interpret_set).token_count));
	if (!((*interpret_set).flag_set))
	{
		gc_collect_all(gc);
		return (1);
	}
	i = 0;
	while (i < ((*interpret_set).token_count))
	{
		(*interpret_set).flag_set[i] = 0;
		i++;
	}
	return (0);
}

char	*strip_quotes(char *s, t_interpret interpret_data, size_t index)
{
	size_t	i;
	size_t	j;
	char	quote;
	char	*result;

	if (init_vars_strip_quotes(&result, &i, &j, s) != 0)
		return (NULL);
	while (s[i])
	{
		if ((s[i] == '\"') || (s[i] == '\''))
		{
			quote = s[i++];
			if (i < interpret_data.token_count)
				interpret_data.flag_set[index] = 1;
			while (s[i] && (s[i] != quote))
				result[j++] = s[i++];
			if (s[i])
				i++;
		}
		else
			result[j++] = s[i++];
	}
	result[j] = '\0';
	return (result);
}

static int	init_vars_strip_quotes(char **result, size_t *i, size_t *j, char *s)
{
	(*result) = malloc(ft_strlen(s) + 1);
	if (!(*result))
		return (1);
	(*i) = 0;
	(*j) = 0;
	return (0);
}
