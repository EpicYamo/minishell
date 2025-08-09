/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_pt_one.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 03:26:05 by aaycan            #+#    #+#             */
/*   Updated: 2025/08/09 19:31:22 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <unistd.h>
#include <limits.h>

static int	create_tokens_from_input(t_interpret_data_input token_set,
				char **tokens, t_gc *gc, t_env *env_list);
static int	modify_token_and_apply(char **token, t_gc *gc, t_env *env_list,
				t_interpret_data_cursor	data_and_cursor);
static int	handle_env_vars_in_token(char **tokens, t_gc *gc,
				t_env *env_list, size_t *i);

char	**split_tokens(const char *input, t_gc *garbage_c, t_env *env_list,
	t_interpret interpret_set)
{
	char					**tokens;
	size_t					token_count;
	t_interpret_data_input	token_set;

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
	token_set.interpret_data = interpret_set;
	token_set.line = input;
	if (create_tokens_from_input(token_set, tokens, garbage_c, env_list) != 0)
	{
		write(2, "Malloc: tokenization failed\n", 28);
		return (NULL);
	}
	return (tokens);
}

static int	create_tokens_from_input(t_interpret_data_input token_set,
	char **tokens, t_gc *gc, t_env *env_list)
{
	size_t					i;
	size_t					j;
	size_t					token_count;
	t_interpret_data_cursor	data_and_cursor;

	i = -1;
	j = 0;
	token_count = count_tokens(token_set.line);
	while (++i < token_count)
	{
		tokens[i] = extract_token(token_set.line, &j);
		if (!tokens[i])
			return (1);
		gc_add(gc, tokens[i]);
		data_and_cursor.i = &i;
		data_and_cursor.interpret_data = token_set.interpret_data;
		if (modify_token_and_apply(tokens, gc, env_list, data_and_cursor) != 0)
			return (1);
	}
	tokens[i] = NULL;
	return (0);
}

static int	modify_token_and_apply(char **tokens, t_gc *gc, t_env *env_list,
	t_interpret_data_cursor	data_and_cursor)
{
	if (!(((*data_and_cursor.i) > 0)
			&& (!(ft_strcmp(tokens[(*data_and_cursor.i) - 1], "<<")))))
	{
		if (handle_env_vars_in_token(tokens, gc, env_list,
				(data_and_cursor.i)) != 0)
			return (1);
	}
	if (strip_quotes_and_apply_token(gc, &tokens[(*data_and_cursor.i)],
			data_and_cursor.interpret_data, (*data_and_cursor.i)) != 0)
		return (1);
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
	return (0);
}
