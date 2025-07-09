/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_pt_one.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 03:26:05 by aaycan            #+#    #+#             */
/*   Updated: 2025/07/09 21:33:37 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

static int	split_tokens_pt_two(const char *input, char **tokens,
				t_gc *garbage_c, t_env *env_list);
static int	split_tokens_pt_three(char **tokens, t_gc *garbage_c,
				t_env *env_list, size_t *i);

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
		printf("\033[0;31mSystem error: memory allocation ");
		printf("failed in \"split_tokens\" function\033[0m\n");
		return (NULL);
	}
	if (split_tokens_pt_two(input, tokens, garbage_c, env_list) != 0)
	{
		printf("\033[0;31mSystem error: memory allocation ");
		printf("failed in \"split_tokens\" function\033[0m\n");
		return (NULL);
	}
	return (tokens);
}

static int	split_tokens_pt_two(const char *input, char **tokens,
	t_gc *garbage_c, t_env *env_list)
{
	size_t	i;
	size_t	j;
	size_t	token_count;

	i = -1;
	j = 0;
	token_count = count_tokens(input);
	while (++i < token_count)
	{
		tokens[i] = extract_token(input, &j);
		if (!tokens[i])
			return (1);
		gc_add(garbage_c, tokens[i]);
		if (split_tokens_pt_three(tokens, garbage_c, env_list, &i) != 0)
			return (1);
		tokens[i] = strip_quotes(tokens[i]);
		if (!tokens[i])
			return (1);
		gc_add(garbage_c, tokens[i]);
	}
	tokens[i] = NULL;
	return (0);
}

static int	split_tokens_pt_three(char **tokens, t_gc *garbage_c,
	t_env *env_list, size_t *i)
{
	size_t	sign_loc;
	size_t	last_sign;

	last_sign = LONG_MAX;
	sign_loc = check_dollar_sign_existance(tokens[(*i)], &last_sign);
	while (sign_loc)
	{
		tokens[(*i)] = expand_env_vars_if_applicable(tokens[(*i)],
				(sign_loc - 1), env_list);
		if (!tokens[(*i)])
			return (1);
		gc_add(garbage_c, tokens[(*i)]);
		sign_loc = check_dollar_sign_existance(tokens[(*i)], &last_sign);
	}
	return (0);
}
