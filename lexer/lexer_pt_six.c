/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_pt_six.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 03:26:14 by aaycan            #+#    #+#             */
/*   Updated: 2025/07/09 21:39:05 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdlib.h>

static int	calculate_env_size_pt_two(const char *token, size_t start,
				size_t env_var_size, t_env *env_list);

int	calculate_env_size(const char *token, t_env *env_list)
{
	size_t	i;
	size_t	env_var_size;
	size_t	start;

	i = 0;
	while (token[i])
	{
		if (token[i] == '$')
			break ;
		i++;
	}
	env_var_size = 0;
	i += 1;
	start = i;
	while (is_env_char(token[i]))
	{
		i++;
		env_var_size++;
	}
	env_var_size = calculate_env_size_pt_two(token, start,
			env_var_size, env_list);
	return (env_var_size);
}

static int	calculate_env_size_pt_two(const char *token, size_t start,
	size_t env_var_size, t_env *env_list)
{
	char	key[env_var_size + 1];
	size_t	i;
	size_t	j;
	size_t	size_of_env;

	i = start;
	j = 0;
	while (i < (start + env_var_size))
	{
		key[j] = token[i];
		i++;
		j++;
	}
	key[j] = '\0';
	size_of_env = ft_strlen(get_minishell_env(key, env_list));
	return (size_of_env);
}

void	env_token_exit_status(char *result, const char *token, size_t cursor)
{
	size_t	j;
	size_t	i;

	j = 0;
	i = cursor + 2;
	while (EXIT_STATUS_TOKEN[j])
	{
		result[cursor] = EXIT_STATUS_TOKEN[j];
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

void	env_token_digit(char *result, const char *token, size_t cursor)
{
	size_t	i;

	i = cursor + 2;
	while (token[i])
	{
		result[cursor] = token[i];
		cursor++;
		i++;
	}
	result[cursor] = '\0';
}
