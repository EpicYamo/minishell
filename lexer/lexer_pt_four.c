/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_pt_four.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 03:25:59 by aaycan            #+#    #+#             */
/*   Updated: 2025/08/08 15:12:15 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdlib.h>

static char	*build_expanded_string(const char *token, size_t loc,
				t_env *env_list, size_t last_sign);
static int	build_expanded_string_pt_two(char *result, const char *token,
				t_env_bounds bounds, t_env *env_list);

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
	t_env *env_list, size_t last_sign)
{
	char	*new_token;

	if (!token)
		return (NULL);
	new_token = build_expanded_string(token, loc, env_list, last_sign);
	if (!new_token)
		return (NULL);
	return (new_token);
}

static char	*build_expanded_string(const char *token, size_t loc,
	t_env *env_list, size_t last_sign)
{
	size_t			result_size;
	char			*result;
	size_t			i;
	int				env_size;
	t_env_bounds	bounds;

	env_size = calculate_env_size(token, env_list, loc);
	if (env_size == -42)
		return (NULL);
	result_size = (env_size + ft_strlen(token) + 60);
	result = malloc(sizeof(char) * result_size);
	if (!result)
		return (NULL);
	i = -1;
	while (++i < loc)
		result[i] = token[i];
	bounds.start_loc = i;
	bounds.end_loc = last_sign;
	if (build_expanded_string_pt_two(result, token, bounds, env_list) != 0)
	{
		free(result);
		return (NULL);
	}
	return (result);
}

static int	build_expanded_string_pt_two(char *result, const char *token,
	t_env_bounds bounds, t_env *env_list)
{
	size_t			i;
	t_lexer_data	data;

	i = bounds.start_loc + 1;
	if (token[i] && (token[i] == '?'))
		env_token_exit_status(result, token, bounds.start_loc);
	else if (token[i] && ft_isdigit(token[i]))
		env_token_digit(result, token, bounds.start_loc);
	else if (token[i] && is_env_char(token[i]))
	{
		data.key_size = i;
		while (is_env_char(token[i]) && (i < bounds.end_loc))
			i++;
		data.key_size = i - data.key_size;
		data.cursor = bounds.start_loc;
		data.end_loc = bounds.end_loc;
		if (env_token_default(result, token, env_list, data) != 0)
			return (1);
	}
	else if (token[i] && (token[i] == '\'' || token[i] == '\"'))
		quoted_env_token(result, token, bounds.start_loc);
	else
		no_valid_env_token(result, token, bounds.start_loc);
	return (0);
}
