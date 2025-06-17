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
static int	check_dollar_sign_existance(char *token);
static void	check_dollar_sign_pt_two(size_t *valid_sign, char c, size_t *i);

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
		while (check_dollar_sign_existance(tokens[i]))
		{
			tokens[i] = expand_env_vars_if_applicable(tokens[i]);
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

static int	check_dollar_sign_existance(char *token)
{
	size_t	i;
	size_t	valid_sign;
	char	quote;

	i = 0;
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
					valid_sign += 1;
				i++;
			}
			if (token[i])
				i++;
		}
		else
			check_dollar_sign_pt_two(&valid_sign, token[i], &i);
	}
	return (valid_sign);
}

static void	check_dollar_sign_pt_two(size_t *valid_sign, char c, size_t *i)
{
	if (c == '$')
		(*valid_sign) += 1;
	(*i) += 1;
}
