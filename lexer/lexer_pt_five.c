/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_pt_five.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 21:39:14 by aaycan            #+#    #+#             */
/*   Updated: 2025/07/12 19:39:54 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdlib.h>

static size_t	env_token_default_pt_two(char *env_ptr, char *result,
					size_t *j, t_lexer_data data);
static void		env_token_default_pt_three(const char *token, char *result,
					t_lexer_data data, int i);

int	env_token_default(char *result, const char *token,
	t_env *env_list, t_lexer_data data)
{
	char	*key;
	char	*env_ptr;
	size_t	i;
	size_t	j;

	key = malloc(sizeof(char) * ((data.key_size) + 1));
	if (!key)
		return (1);
	i = (data.cursor) + 1;
	j = 0;
	while (is_env_char(token[i]))
	{
		key[j] = token[i];
		i++;
		j++;
	}
	key[j] = '\0';
	env_ptr = get_minishell_env(key, env_list);
	free(key);
	data.cursor = env_token_default_pt_two(env_ptr, result, &j, data);
	env_token_default_pt_three(token, result, data, i);
	return (0);
}

static size_t	env_token_default_pt_two(char *env_ptr, char *result,
	size_t *j, t_lexer_data data)
{
	if (env_ptr)
	{
		(*j) = 0;
		while (env_ptr[(*j)])
		{
			result[(data.cursor)] = env_ptr[(*j)];
			(data.cursor)++;
			(*j)++;
		}
	}
	return (data.cursor);
}

static void	env_token_default_pt_three(const char *token, char *result,
	t_lexer_data data, int i)
{
	while (token[i])
	{
		result[(data.cursor)] = token[i];
		(data.cursor)++;
		i++;
	}
	result[(data.cursor)] = '\0';
}

void	no_valid_env_token(char *result, const char *token, size_t cursor)
{
	size_t	j;
	size_t	i;

	j = 0;
	i = cursor + 1;
	while (DOLLAR_SIGN_TOKEN[j])
	{
		result[cursor] = DOLLAR_SIGN_TOKEN[j];
		cursor++;
		j++;
	}
	while (token[i])
	{
		result[cursor] = token[i];
		cursor++;
		i++;
	}
	result[cursor] = '\0';
}
