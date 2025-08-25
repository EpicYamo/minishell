/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_pt_five.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 23:23:55 by aaycan            #+#    #+#             */
/*   Updated: 2025/08/25 19:43:23 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdlib.h>

static int	apply_status_token(char *replacement, t_command *cmd,
				t_parser_cursor *cursor);
static void	replace_argv(char **argv, char *replacement,
				t_parser_cursor cursor);

int	apply_exit_status_token(t_command *cmd, t_gc *gc)
{
	int	i;

	i = 0;
	while (cmd->argv[i])
	{
		if (check_status_sign_existance(cmd->argv[i]) != 0)
		{
			if (apply_status_sign(&(cmd->argv[i]), cmd, gc) != 0)
				return (1);
		}
		i++;
	}
	return (0);
}

int	apply_status_sign(char **argv, t_command *cmd, t_gc *gc)
{
	char			*replacement;
	t_parser_cursor	cursor;

	replacement = gc_malloc(gc, (sizeof(char) * (ft_strlen(*argv) + 1)));
	if (!replacement)
		return (1);
	cursor.i = 0;
	cursor.argc = 0;
	while (argv[0][cursor.i])
	{
		if (ft_strncmp(&argv[0][cursor.i], EXIT_STATUS_TOKEN,
			ft_strlen(EXIT_STATUS_TOKEN)) == 0)
		{
			if (apply_status_token(replacement, cmd, &cursor) != 0)
				return (1);
		}
		else
		{
			replacement[cursor.argc] = argv[0][cursor.i];
			(cursor.argc)++;
			(cursor.i)++;
		}
	}
	replace_argv(argv, replacement, cursor);
	return (0);
}

static void	replace_argv(char **argv, char *replacement, t_parser_cursor cursor)
{
	replacement[(cursor.argc)] = '\0';
	(*argv) = replacement;
}

static int	apply_status_token(char *replacement, t_command *cmd,
	t_parser_cursor *cursor)
{
	int		cur;
	char	*status_token;

	status_token = ft_itoa(cmd->io->exit_status);
	if (!status_token)
		return (1);
	cur = 0;
	while (status_token[cur])
	{
		replacement[(*cursor).argc] = status_token[cur];
		cur++;
		(*cursor).argc++;
	}
	free(status_token);
	(*cursor).i += ft_strlen(EXIT_STATUS_TOKEN);
	return (0);
}

int	check_status_sign_existance(char *argv)
{
	int	status_sign_count;
	int	i;

	status_sign_count = 0;
	i = 0;
	while (argv[i])
	{
		if (ft_strncmp(&argv[i], EXIT_STATUS_TOKEN,
				ft_strlen(EXIT_STATUS_TOKEN)) == 0)
			status_sign_count += 1;
		i++;
	}
	return (status_sign_count);
}
