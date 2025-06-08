/* ************************************************************************************** */
/*                                                                                        */
/*                                                                   :::      ::::::::    */
/*   lexer_pt_one.c                                                :+:      :+:    :+:    */
/*                                                               +:+ +:+         +:+      */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr>              +#+  +:+       +#+         */
/*                                                           +#+#+#+#+#+   +#+            */
/*   Created: 2025/06/04 16:04:06 by aaycan                       #+#    #+#              */
/*   Updated: 2025/06/04 16:04:06 by aaycan                      ###   ########.tr        */
/*                                                                                        */
/* ************************************************************************************** */

#include "minishell.h"
#include <stdlib.h>
#include <stdio.h>

static int		has_unclosed_quotes(const char *s);
static size_t	count_tokens(const char *s);
static char		*extract_token(const char *s, size_t *i);
static char		*strip_quotes(char *s);

char	**split_tokens(const char *input)
{
	size_t	token_count;
	char	**tokens;
	size_t	i;
	size_t	t;

	if (has_unclosed_quotes(input))
	{
		printf("\033[0;31mSyntax error: unclosed quote\033[0m\n");
		return (NULL); // do not return NULL, return a key to prevent processing it like an allocation error.
	}
	i = 0;
	t = -1;
	token_count = count_tokens(input);
	tokens = malloc(sizeof(char *) * (token_count + 1));
	if (!tokens)
		return (NULL);
	while (++t < token_count)
	{
		tokens[t] = extract_token(input, &i);
		//if (!tokens[t])
		//free tokens in a loop then free token
		//return NULL
		tokens[t] = strip_quotes(tokens[t]);
		//if (!tokens[t])
		//free tokens in a loop then free token
		//return NULL
	}
	tokens[t] = NULL;
	return (tokens);
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

static char	*extract_token(const char *s, size_t *i)
{
	size_t	start;
	char	quote;
	char 	*token;

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
	token = ft_strndup(s + start, *i - start);
	if (!token)
		return (NULL);
	return (token);
}

static char	*strip_quotes(char *s)
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
		if ((s[i] == '\"') || (s[i] == '\''))
		{
			quote = s[i++];
			while (s[i] && (s[i] != quote))
				result[j++] = s[i++];
			if (s[i])
				i++;
		}
		else
			result[j++] = s[i++];
	}
	result[j] = '\0';
	free(s);
	return (result);
}
