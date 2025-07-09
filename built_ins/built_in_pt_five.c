/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_pt_five.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 22:26:09 by aaycan            #+#    #+#             */
/*   Updated: 2025/07/09 18:53:50 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdio.h>
#include <stdlib.h>

static int	split_key_value_pt_two(char *arg, char **key, char **value);

int	split_key_value(char *arg, char **key, char **value)
{
	char	*eq;

	eq = ft_strchr(arg, '=');
	if (eq)
	{
		*key = ft_strndup(arg, eq - arg);
		if (!(*key))
		{
			printf("ALLOCATION_ERROR for Export Command\n");
			return (1);
		}
		*value = ft_strdup(eq + 1);
		if (!(*value))
		{
			free(key);
			printf("ALLOCATION_ERROR for Export Command\n");
			return (1);
		}
	}
	else
	{
		if (split_key_value_pt_two(arg, key, value) != 0)
			return (1);
	}
	return (0);
}

static int	split_key_value_pt_two(char *arg, char **key, char **value)
{
	*value = NULL;
	*key = ft_strdup(arg);
	if (!(*key))
	{
		printf("ALLOCATION_ERROR for Export Command\n");
		return (1);
	}
	return (0);
}
