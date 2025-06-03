/* ************************************************************************************** */
/*                                                                                        */
/*                                                                   :::      ::::::::    */
/*   lexer.c                                                       :+:      :+:    :+:    */
/*                                                               +:+ +:+         +:+      */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr>              +#+  +:+       +#+         */
/*                                                           +#+#+#+#+#+   +#+            */
/*   Created: 2025/06/04 00:09:48 by aaycan                       #+#    #+#              */
/*   Updated: 2025/06/04 00:09:48 by aaycan                      ###   ########.tr        */
/*                                                                                        */
/* ************************************************************************************** */

#include "minishell.h"
#include "libraries/libft.h"
#include <stdlib.h>
#include <stdio.h>

static size_t	count_tokens(const char *s);
static int		is_metachar(char c);
static int		ft_isspace(char c);
static char		*extract_token(const char *s, size_t *i);
static char		*ft_strndup(const char *s, size_t n);
static int		has_unclosed_quotes(const char *s);
static char		*strip_quotes(const char *s);

char	**split_tokens(const char *input)
{
	size_t	token_count;
	char	**tokens;
	size_t	i;
	size_t	t;

	if (has_unclosed_quotes(input))
	{
		printf("\033[0;31mSyntax error: unclosed quote\033[0m\n");
		return (NULL);
	}
	i = 0;
	t = 0;
	token_count = count_tokens(input);
	tokens = malloc(sizeof(char *) * (token_count + 1));
	if (!tokens)
		return (NULL);
	while (t < token_count)
		tokens[t++] = strip_quotes(extract_token(input, &i));
	tokens[t] = NULL;
	return (tokens);
}

static size_t	count_tokens(const char *s)
{
	size_t	count;
	size_t	i;
	char	quote;

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
		{
			while (s[i] && !ft_isspace(s[i]) && !is_metachar(s[i]))
			{
				if ((s[i] == '\"') || (s[i] == '\''))
				{
					quote = s[i++];
					while (s[i] && (s[i] != quote))
						i++;
					if (s[i])
						i++;
				}
				else
					i++;
			}
		}
		count++;
	}
	return (count);
}

static int	is_metachar(char c)
{
	return ((c == '|') || (c == '<') || (c == '>'));
}

static int	ft_isspace(char c)
{
	return ((c == ' ') || (c >= 9 && c <= 13));
}

static char	*ft_strndup(const char *s, size_t n)
{
	char *dup;
	size_t i;

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

static char	*extract_token(const char *s, size_t *i)
{
	size_t	start;
	char	quote;

	while (ft_isspace(s[*i]))
		(*i)++;
	start = *i;
	if (((s[*i] == '<') && (s[*i + 1] == '<')) || ((s[*i] == '>') && (s[*i + 1] == '>')))
		(*i) += 2;
	else if (is_metachar(s[*i]))
		(*i)++;
	else
	{
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
	return (ft_strndup(s + start, *i - start));
}

static int	has_unclosed_quotes(const char *s)
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

static char	*strip_quotes(const char *s)
{
	size_t	i;
	size_t	j;
	char	quote;
	char	*result;

	result = malloc(ft_strlen(s) + 1);
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	while (s[i])
	{
		if ((s[i] == '"') || (s[i] == '\''))
		{
			quote = s[i++];
			while (s[i] && s[i] != quote)
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
