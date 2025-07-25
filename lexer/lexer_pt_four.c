/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_pt_four.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 03:25:59 by aaycan            #+#    #+#             */
/*   Updated: 2025/07/12 19:31:53 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdlib.h>

static char	*build_expanded_string(const char *token, size_t loc,
				t_env *env_list);
static int	build_expanded_string_pt_two(char *result, const char *token,
				size_t cursor, t_env *env_list);

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

char	*expand_env_vars_if_applicable(const char *token, size_t loc,
	t_env *env_list)
{
	char	*new_token;

	if (!token)
		return (NULL);
	new_token = build_expanded_string(token, loc, env_list);
	if (!new_token)
		return (NULL);
	return (new_token);
}

static char	*build_expanded_string(const char *token, size_t loc,
	t_env *env_list)
{
	size_t	result_size;
	char	*result;
	size_t	i;
	int		env_size;

	env_size = calculate_env_size(token, env_list, loc);
	if (env_size == -42)
		return (NULL);
	result_size = (env_size + ft_strlen(token) + 60);
	result = malloc(sizeof(char) * result_size);
	if (!result)
		return (NULL);
	i = 0;
	while (i < loc)
	{
		result[i] = token[i];
		i++;
	}
	if (build_expanded_string_pt_two(result, token, i, env_list) != 0)
	{
		free(result);
		return (NULL);
	}
	return (result);
}

static int	build_expanded_string_pt_two(char *result, const char *token,
	size_t cursor, t_env *env_list)
{
	size_t			i;
	size_t			key_size;
	t_lexer_data	data;

	i = cursor + 1;
	if (token[i] && (token[i] == '?'))
		env_token_exit_status(result, token, cursor);
	else if (token[i] && ft_isdigit(token[i]))
		env_token_digit(result, token, cursor);
	else if (token[i] && is_env_char(token[i]))
	{
		key_size = 0;
		while (is_env_char(token[i]))
		{
			i++;
			key_size++;
		}
		data.cursor = cursor;
		data.key_size = key_size;
		if (env_token_default(result, token, env_list, data) != 0)
			return (1);
	}
	else
		no_valid_env_token(result, token, cursor);
	return (0);
}
