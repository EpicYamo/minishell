/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_pt_nine.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 04:59:57 by aaycan            #+#    #+#             */
/*   Updated: 2025/08/14 05:04:12 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <limits.h>

static void	check_dollar_sign_pt_two(size_t *valid_sign, char *token,
				size_t *loc, size_t *last_sign);
static void	check_dollar_sign_pt_three(char *token, size_t *last_sign,
				size_t *valid_sign, t_lexer_cursor *cursor);

size_t	check_dollar_sign_existance_heredoc(char *token, size_t *last_sign)
{
	size_t	i;
	size_t	valid_sign;
	size_t	loc;

	i = 0;
	valid_sign = 0;
	if (((*last_sign) != LLONG_MAX))
	{
		while (token[i])
		{
			if ((token[i] == '$') && (i == ((*last_sign) - 1)))
			{
				(*last_sign) = i;
				i = 0;
			}
			else
				i++;
		}
	}
	check_dollar_sign_pt_two(&valid_sign, token, &loc, last_sign);
	return (valid_sign);
}

static void	check_dollar_sign_pt_two(size_t *valid_sign, char *token,
	size_t *loc, size_t *last_sign)
{
	t_lexer_cursor	cursor;

	cursor.i = 0;
	cursor.loc = loc;
	while (token[(cursor.i)])
	{
		if ((token[(cursor.i)] == '\"') || (token[(cursor.i)] == '\''))
			check_dollar_sign_pt_three(token, last_sign, valid_sign, &cursor);
		else
		{
			if ((token[(cursor.i)] == '$') && ((cursor.i) < (*last_sign)))
			{
				while (token[(cursor.i) + 1] && (token[(cursor.i) + 1] == '$'))
					(cursor.i)++;
				(*valid_sign) += 1;
				(*loc) = (cursor.i);
			}
			(cursor.i) += 1;
		}
	}
	if (*valid_sign)
	{
		(*last_sign) = (*loc);
		(*valid_sign) = ((*loc) + 1);
	}
}

static void	check_dollar_sign_pt_three(char *token, size_t *last_sign,
	size_t *valid_sign, t_lexer_cursor *cursor)
{
	char	quote;

	quote = token[(cursor->i)++];
	while (token[(cursor->i)] && (token[(cursor->i)] != quote))
	{
		if ((token[(cursor->i)] == '$')
			&& (quote == '\"' || quote == '\'') && ((cursor->i) < (*last_sign)))
		{
			while ((token[(cursor->i) + 1]) && (token[(cursor->i) + 1] == '$'))
				(cursor->i)++;
			(*valid_sign) += 1;
			(*cursor->loc) = (cursor->i);
		}
		(cursor->i)++;
	}
	if (token[(cursor->i)])
		(cursor->i)++;
}
