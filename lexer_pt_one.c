/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_pt_one.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 03:26:05 by aaycan            #+#    #+#             */
/*   Updated: 2025/07/08 17:22:10 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

static int		split_tokens_pt_two(const char *input, char **tokens, size_t token_count, t_gc *garbage_c, t_env *env_list);
static size_t	check_dollar_sign_existance(char *token, size_t *last_sign);
static void		check_dollar_sign_pt_two(size_t *valid_sign, char *token, size_t *i, size_t *loc, size_t *last_sign);

char	**split_tokens(const char *input, t_gc *garbage_c, t_env *env_list)
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
	if (split_tokens_pt_two(input, tokens, token_count, garbage_c, env_list) != 0)
	{
		printf("\033[0;31mSystem error: memory allocation failed in \"split_tokens\" function\033[0m\n");
		return (NULL);
	}
	return (tokens);
}

static int	split_tokens_pt_two(const char *input, char **tokens, size_t token_count, t_gc *garbage_c, t_env *env_list)
{
	size_t	i;
	size_t	j;
	size_t	last_sign;
	size_t	sign_loc;

	i = -1;
	j = 0;
	while (++i < token_count)
	{
		last_sign = LONG_MAX;
		tokens[i] = extract_token(input, &j);
		if (!tokens[i])
			return (1);
		gc_add(garbage_c, tokens[i]);
		sign_loc = check_dollar_sign_existance(tokens[i], &last_sign);
		while (sign_loc)
		{
			tokens[i] = expand_env_vars_if_applicable(tokens[i], (sign_loc - 1), env_list);
			if (!tokens[i])
				return (1);
			gc_add(garbage_c, tokens[i]);
			sign_loc = check_dollar_sign_existance(tokens[i], &last_sign);
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

	i = 0;
	valid_sign = 0;
	quote = 0;
	if (((*last_sign) != LLONG_MAX))
	{
		while (token[i])
		{
			if ((token[i] == '$') && (i == ((*last_sign ) - 1)))
			{
				(*last_sign) = i;
				i = 0;
			}
			else
				i++;
		}
	}
	i = 0;
	while (token[i])
	{
		if ((token[i] == '\"') || (token[i] == '\''))
		{	
			quote = token[i++];
			while (token[i] && (token[i] != quote))
			{
				if ((token[i] == '$') && (quote == '\"') && (i < (*last_sign)))
				{
					while ((token[i + 1]) && (token[i + 1] == '$'))
						i++;
					valid_sign += 1;
					loc = i;
				}
				i++;
			}
			if (token[i])
				i++;
		}
		else
			check_dollar_sign_pt_two(&valid_sign, token, &i, &loc, last_sign);
	}
	if (valid_sign)
	{
		(*last_sign) = loc;
		return (loc + 1);
	}
	return (valid_sign);
}

static void	check_dollar_sign_pt_two(size_t *valid_sign, char *token, size_t *i, size_t *loc, size_t *last_sign)
{
	if ((token[(*i)] == '$') && ((*i) < (*last_sign)))
	{
		while (token[(*i) + 1] && (token[(*i) + 1] == '$'))
			(*i)++;
		(*valid_sign) += 1;
		(*loc) = (*i);
	}
	(*i) += 1;
}
