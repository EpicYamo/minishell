/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_pt_eight.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 14:52:11 by aaycan            #+#    #+#             */
/*   Updated: 2025/08/06 14:53:09 by aaycan           ###   ########.fr       */
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
