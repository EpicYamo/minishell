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

static int		split_tokens_pt_two(const char *input, char **tokens, size_t token_count, t_gc *garbage_c);
static size_t	check_dollar_sign_existance(char *token, size_t *last_sign);
static void		check_dollar_sign_pt_two(size_t *valid_sign, char *token, size_t *i, size_t *loc);

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
	size_t	last_sign;

	i = -1;
	j = 0;
	last_sign = 0;
	while (++i < token_count)
	{
		tokens[i] = extract_token(input, &j);
		if (!tokens[i])
			return (1);
		gc_add(garbage_c, tokens[i]);
		while (check_dollar_sign_existance(tokens[i], &last_sign))
		{
			tokens[i] = expand_env_vars_if_applicable(tokens[i], (check_dollar_sign_existance(tokens[i], &last_sign) - 1));
			if (!tokens[i])
				return (1);
			gc_add(garbage_c, tokens[i]);
		}
		tokens[i] = strip_quotes(tokens[i]);
		if (!tokens[i])
			return (1);
		gc_add(garbage_c, tokens[i]);
	}
	tokens[i] = NULL;
	return (0);
}

static size_t	check_dollar_sign_existance(char *token, size_t *last_sign)
{
	size_t	i;
	size_t	valid_sign;
	char	quote;
	size_t	loc;

	i = (*last_sign);
	valid_sign = 0;
	quote = 0;
	while (token[i])
	{
		if ((token[i] == '\"') || (token[i] == '\''))
		{	
			quote = token[i++];
			while (token[i] && (token[i] != quote))
			{
				if ((token[i] == '$') && (quote == '\"'))
				{
					while ((token[i + 1]) && (token[i + 1] == '$'))
						i++;
					valid_sign++;
					loc = i;
				}
				i++;
			}
			if (token[i])
				i++;
		}
		else
			check_dollar_sign_pt_two(&valid_sign, token, &i, &loc);
	}
	if (valid_sign)
	{
		(*last_sign) = loc;
		return (loc + 1);
	}
	return (valid_sign);
}

static void	check_dollar_sign_pt_two(size_t *valid_sign, char *token, size_t *i, size_t *loc)
{
	if (token[(*i)] == '$')
	{
		while (token[(*i) + 1] && (token[(*i) + 1] == '$'))
			(*i)++;
		(*valid_sign) = 1;
		(*loc) = (*i);
	}
	(*i) += 1;
}
