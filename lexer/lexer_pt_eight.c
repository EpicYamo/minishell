/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_pt_eight.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 14:52:11 by aaycan            #+#    #+#             */
/*   Updated: 2025/08/09 18:53:50 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	strip_quotes_and_apply_token(t_gc *gc, char **token)
{
	(*token) = strip_quotes(*token);
	if (!(*token))
		return (1);
	gc_add(gc, (*token));
	return (0);
}

int	init_interpret_set(t_gc *gc, t_interpret *interpret_set, char *line)
{
	size_t	i;

	(*interpret_set).token_count = count_tokens(line);
	(*interpret_set).flag_set = gc_malloc(gc,
			(sizeof(size_t) * (*interpret_set).token_count));
	if (!((*interpret_set).flag_set))
	{
		gc_collect_all(gc);
		return (1);
	}
	i = 0;
	while (i < ((*interpret_set).token_count))
	{
		(*interpret_set).flag_set[i] = 0;
		i++;
	}
	return (0);
}
