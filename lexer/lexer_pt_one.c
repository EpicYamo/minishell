/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_pt_one.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 03:26:05 by aaycan            #+#    #+#             */
/*   Updated: 2025/08/04 18:49:39 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdlib.h>
#include <unistd.h>
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
		write(2, "Syntax error: unclosed quote\n", 29);
		return (NULL);
	}
	token_count = count_tokens(input);
	tokens = gc_malloc(garbage_c, (sizeof(char *) * (token_count + 1)));
	if (!tokens)
	{
		write(2, "Malloc: tokenization failed\n", 28);
		return (NULL);
	}
	if (split_tokens_pt_two(input, tokens, garbage_c, env_list) != 0)
	{
		write(2, "Malloc: tokenization failed\n", 28);
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
		if (!((i > 0) && (!(ft_strcmp(tokens[i - 1], "<<")))))
		{
			if (split_tokens_pt_three(tokens, garbage_c, env_list, &i) != 0)
				return (1);
		}
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
	tokens[(*i)] = replace_dollar_signs(tokens[(*i)], garbage_c);
	if (!tokens[(*i)])
		return (1);
	return (0);
}
