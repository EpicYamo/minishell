/* ************************************************************************************** */
/*                                                                                        */
/*                                                                   :::      ::::::::    */
/*   lexer_pt_three.c                                              :+:      :+:    :+:    */
/*                                                               +:+ +:+         +:+      */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr>              +#+  +:+       +#+         */
/*                                                           +#+#+#+#+#+   +#+            */
/*   Created: 2025/06/16 16:22:35 by aaycan                       #+#    #+#              */
/*   Updated: 2025/06/16 16:22:35 by aaycan                      ###   ########.tr        */
/*                                                                                        */
/* ************************************************************************************** */

#include "minishell.h"
#include <stdlib.h>
#include <stdio.h>

static char	*build_expanded_string(const char *token, size_t loc);
static void	build_expanded_string_pt_two(char *result, const char *token, size_t cursor);
static void	env_token_default(char *result, const char *token, size_t cursor, size_t key_size);

char	*strip_quotes(char *s)
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
	return (result);
}

char	*expand_env_vars_if_applicable(const char *token, size_t loc)
{
	char	*new_token;

	if (!token)
		return (NULL);
	new_token = build_expanded_string(token, loc);
	if (!new_token)
		return (NULL);
	return (new_token);
}

static char	*build_expanded_string(const char *token, size_t loc)
{
	size_t	result_size;
	char	*result;
	size_t	i;

	result_size = (calculate_env_size(token) + ft_strlen(token) + 30);
	result = malloc(sizeof(char) * result_size);
	if (!result)
		return (NULL);
	i = 0;
	while (i < loc)
	{
		result[i] = token[i];
		i++;
	}
	build_expanded_string_pt_two(result, token, i);
	return (result);
}

static void	build_expanded_string_pt_two(char *result, const char *token, size_t cursor)
{
	size_t	i;
	size_t	key_size;

	i = cursor + 1;
	if (token[i] && (token[i] == '?'))
		env_token_exit_status(result, token, cursor);
	else if (token[i] && ft_isdigit(token[i]))
		env_token_digit(result, token, cursor);
	else
	{
		key_size = 0;
		while (is_env_char(token[i]))
		{
			i++;
			key_size++;
		}
		env_token_default(result, token, cursor, key_size);
	}
}

static void	env_token_default(char *result, const char *token, size_t cursor, size_t key_size)
{
	char	key[key_size + 1];
	char	*env_ptr;
	size_t	i;
	size_t	j;

	i = cursor + 1;
	j = 0;
	while (is_env_char(token[i]))
	{
		key[j] = token[i];
		i++;
		j++;
	}
	key[j] = '\0';
	env_ptr = getenv(key);
	if (env_ptr)
	{
		j = 0;
		while (env_ptr[j])
		{
			result[cursor] = env_ptr[j];
			cursor++;
			j++;
		}
	}
	while (token[i])
	{
		result[cursor] = token[i];
		cursor++;
		i++;
	}
	result[cursor] = '\0';
}
