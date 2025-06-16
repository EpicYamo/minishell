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

static int	split_tokens_pt_two(const char *input, char **tokens, size_t token_count, t_gc *garbage_c);
static char	*extract_token(const char *s, size_t *i);
static void	extract_token_pt_two(const char *s, size_t *i);

char	**split_tokens(const char *input, t_gc *garbage_c)
{
	char	**tokens;
	size_t	token_count;

	if (has_unclosed_quotes(input))
	{
		printf("\033[0;31mSyntax error: unclosed quote\033[0m\n");
		return (NULL);
	}
	token_count = count_tokens(input);
	tokens = gc_malloc(garbage_c, (sizeof(char *) * (token_count + 1)));
	if (!tokens)
	{
		printf("\033[0;31mSystem error: memory allocation failed in \"split_tokens\" function\033[0m\n");
		return (NULL);
	}
	if (split_tokens_pt_two(input, tokens, token_count, garbage_c) != 0)
	{
		printf("\033[0;31mSystem error: memory allocation failed in \"split_tokens\" function\033[0m\n");
		return (NULL);
	}
	return (tokens);
}

static int	split_tokens_pt_two(const char *input, char **tokens, size_t token_count, t_gc *garbage_c)
{
	size_t	i;
	size_t	j;

	i = -1;
	j = 0;
	while (++i < token_count)
	{
		tokens[i] = extract_token(input, &j);
		if (!tokens[i])
			return (1);
		gc_add(garbage_c, tokens[i]);
		tokens[i] = strip_quotes(tokens[i]);
		if (!tokens[i])
			return (1);
		gc_add(garbage_c, tokens[i]);
	}
	tokens[i] = NULL;
	return (0);
}

static char	*extract_token(const char *s, size_t *i)
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
