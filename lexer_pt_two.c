/* ************************************************************************************** */
/*                                                                                        */
/*                                                                   :::      ::::::::    */
/*   lexer_pt_two.c                                                :+:      :+:    :+:    */
/*                                                               +:+ +:+         +:+      */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr>              +#+  +:+       +#+         */
/*                                                           +#+#+#+#+#+   +#+            */
/*   Created: 2025/06/16 00:30:19 by aaycan                       #+#    #+#              */
/*   Updated: 2025/06/16 00:30:19 by aaycan                      ###   ########.tr        */
/*                                                                                        */
/* ************************************************************************************** */

#include "minishell.h"
#include <stdlib.h>

static void	count_tokens_pt_two(const char *s, size_t *i);
static void	extract_token_pt_two(const char *s, size_t *i);

int	has_unclosed_quotes(const char *s)
{
	char	quote;
	size_t	i;

	quote = 0;
	i = 0;
	while (s[i])
	{
		if (!quote && ((s[i] == '\'') || (s[i] == '\"')))
			quote = s[i];
		else if (quote && (s[i] == quote))
			quote = 0;
		i++;
	}
	return (quote != 0);
}

size_t	count_tokens(const char *s)
{
	size_t	count;
	size_t	i;

	count = 0;
	i = 0;
	while (s[i])
	{
		while (ft_isspace(s[i]))
			i++;
		if (!s[i])
			break;
		if (((s[i] == '<') && (s[i + 1] == '<')) || ((s[i] == '>') && (s[i + 1] == '>')))
			i += 2;
		else if (is_metachar(s[i]))
			i++;
		else
			count_tokens_pt_two(s, &i);
		count++;
	}
	return (count);
}

static void	count_tokens_pt_two(const char *s, size_t *i)
{
	char	quote;

	while (s[(*i)] && !ft_isspace(s[(*i)]) && !is_metachar(s[(*i)]))
	{
		if ((s[(*i)] == '\"') || (s[(*i)] == '\''))
		{
			quote = s[(*i)++];
			while (s[(*i)] && (s[(*i)] != quote))
				(*i)++;
			if (s[*(i)])
				(*i)++;
		}
		else
			(*i)++;
	}
}

char	*extract_token(const char *s, size_t *i)
{
	size_t	start;
	char	*token;

	while (ft_isspace(s[*i]))
		(*i)++;
	start = *i;
	if (((s[*i] == '<') && (s[*i + 1] == '<')) || ((s[*i] == '>') && (s[*i + 1] == '>')))
		(*i) += 2;
	else if (is_metachar(s[*i]))
		(*i)++;
	else
		extract_token_pt_two(s, i);
	token = ft_strndup(s + start, *i - start);
	if (!token)
		return (NULL);
	return (token);
}

static void	extract_token_pt_two(const char *s, size_t *i)
{
	char	quote;

	while (s[*i] && !ft_isspace(s[*i]) && !is_metachar(s[*i]))
	{
		if ((s[*i] == '\"') || (s[*i] == '\''))
		{
			quote = s[(*i)++];
			while (s[*i] && (s[*i] != quote))
				(*i)++;
			if (s[*i])
				(*i)++;
		}
		else
			(*i)++;
	}
}
