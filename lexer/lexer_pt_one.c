/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_pt_one.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 03:26:05 by aaycan            #+#    #+#             */
/*   Updated: 2025/08/08 15:16:20 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <stdio.h>

static int	create_tokens_from_input(const char *input, char **tokens,
				t_gc *gc, t_env *env_list);
static int	modify_token_and_apply(char **token, t_gc *gc, t_env *env_list,
				size_t *i);
static int	handle_env_vars_in_token(char **tokens, t_gc *gc,
				t_env *env_list, size_t *i);

char	**split_tokens(const char *input, t_gc *garbage_c, t_env *env_list)
{
	char	**tokens;
	size_t	token_count;

	if (has_unclosed_quotes(input))
	{
		write(2, "Syntax error: unclosed quote\n", 29);
		return (NULL);
	}
	token_count = count_tokens(input);
	tokens = gc_malloc(garbage_c, (sizeof(char *) * ((2 * token_count) + 1)));
	if (!tokens)
	{
		write(2, "Malloc: tokenization failed\n", 28);
		return (NULL);
	}
	if (create_tokens_from_input(input, tokens, garbage_c, env_list) != 0)
	{
		write(2, "Malloc: tokenization failed\n", 28);
		return (NULL);
	}
	return (tokens);
}

static int	create_tokens_from_input(const char *input, char **tokens,
	t_gc *gc, t_env *env_list)
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
		gc_add(gc, tokens[i]);
		if (modify_token_and_apply(tokens, gc, env_list, &i) != 0)
			return (1);
	}
	tokens[i] = NULL;
	return (0);
}

static int	modify_token_and_apply(char **tokens, t_gc *gc, t_env *env_list,
	size_t *i)
{
	if (((*i) > 0) && (!(ft_strcmp(tokens[(*i) - 1], "<<"))))
	{
		if (strip_quotes_and_apply_token(gc, &tokens[(*i)]) != 0)
			return (1);
	}
	else
	{
		if (handle_env_vars_in_token(tokens, gc, env_list, i) != 0)
			return (1);
	}
	return (0);
}

static int	handle_env_vars_in_token(char **tokens, t_gc *gc,
	t_env *env_list, size_t *i)
{
	size_t	sign_loc;
	size_t	last_sign;
	size_t	prev_sign;

	last_sign = LONG_MAX;
	prev_sign = last_sign;
	sign_loc = check_dollar_sign_existance(tokens[(*i)], &last_sign);
	while (sign_loc)
	{
		tokens[(*i)] = expand_env_vars_if_applicable(tokens[(*i)],
				(sign_loc - 1), env_list, prev_sign);
		if (!tokens[(*i)])
			return (1);
		gc_add(gc, tokens[(*i)]);
		prev_sign = last_sign;
		sign_loc = check_dollar_sign_existance(tokens[(*i)], &last_sign);
	}
	tokens[(*i)] = replace_dollar_signs(tokens[(*i)], gc);
	if (!tokens[(*i)])
		return (1);
	if (strip_quotes_and_apply_token(gc, &tokens[(*i)]) != 0)
		return (1);
	return (0);
}
