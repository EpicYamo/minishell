/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_pt_five.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 21:39:14 by aaycan            #+#    #+#             */
/*   Updated: 2025/07/09 22:06:44 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static size_t	env_token_default_pt_two(char *env_ptr, char *result,
					size_t *j, t_lexer_data data);

void	env_token_default(char *result, const char *token,
	t_env *env_list, t_lexer_data data)
{
	char	key[(data.key_size) + 1];
	char	*env_ptr;
	size_t	i;
	size_t	j;

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
	data.cursor = env_token_default_pt_two(env_ptr, result, &j, data);
	while (token[i])
	{
		result[(data.cursor)] = token[i];
		(data.cursor)++;
		i++;
	}
	result[(data.cursor)] = '\0';
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
