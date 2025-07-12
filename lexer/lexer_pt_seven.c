/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_pt_seven.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 19:41:41 by aaycan            #+#    #+#             */
/*   Updated: 2025/07/12 22:10:32 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdlib.h>

static char	*replace_dollar_sign_flag(char *token, int loc);

char	*replace_dollar_signs(char *token, t_gc *gc)
{
	int		loc;

	loc = search_str_in_str(token, DOLLAR_SIGN_TOKEN);
	while (loc)
	{
		token = replace_dollar_sign_flag(token, loc - 1);
		if (!token)
			return (NULL);
		gc_add(gc, token);
		loc = search_str_in_str(token, DOLLAR_SIGN_TOKEN);
	}
	return (token);
}

static char	*replace_dollar_sign_flag(char *token, int loc)
{
	char	*result;
	int		i;
	int		start;

	result = malloc(sizeof(char) * ft_strlen(token));
	if (!result)
		return (NULL);
	i = -1;
	while (++i < loc)
		result[i] = token[i];
	result[i] = '$';
	start = i + 1;
	i += 34;
	while (token[i])
	{
		result[start] = token[i];
		start++;
		i++;
	}
	result[start] = '\0';
	return (result);
}
