/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_pt_one.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 02:34:55 by aaycan            #+#    #+#             */
/*   Updated: 2025/07/09 22:07:48 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdlib.h>
#include <stdio.h>

static void	init_variables(t_parser_cursor *cursor, t_command **head,
				t_command **cmd);
static int	initialize_command_if_needed(t_command **cmd, t_command **head,
				t_parser_cursor *cursor, t_gc *gc);
static int	handle_pipe_token(char **tokens, t_command **cmd, t_gc *gc,
				t_parser_cursor *cursor);
static int	append_token_to_argv(t_command *cmd, char *token, t_gc *gc,
				t_parser_cursor *cursor);

t_command	*parse_tokens(char **tokens, t_gc *gc)
{
	t_command		*head;
	t_command		*cmd;
	t_parser_cursor	cursor;
	int				status;

	init_variables(&cursor, &cmd, &head);
	while (tokens[cursor.i])
	{
		if (!initialize_command_if_needed(&cmd, &head, &cursor, gc))
			return (NULL);
		status = handle_pipe_token(tokens, &cmd, gc, &cursor);
		if (status == 1)
			continue ;
		else if (status == -1)
			return (NULL);
		status = handle_redirection_token(tokens, cmd, gc, &cursor);
		if (status == -1)
			return (NULL);
		else if (status == 1)
			continue ;
		if (!append_token_to_argv(cmd, tokens[(cursor.i)], gc, &cursor))
			return (NULL);
		(cursor.i)++;
	}
	return (head);
}

static void	init_variables(t_parser_cursor *cursor,
	t_command **head, t_command **cmd)
{
	cursor->i = 0;
	cursor->argc = 0;
	*head = NULL;
	*cmd = NULL;
}

static int	initialize_command_if_needed(t_command **cmd, t_command **head,
	t_parser_cursor *cursor, t_gc *gc)
{
	if (!(*cmd) && !(*head))
	{
		*head = new_command(gc);
		if (!(*head))
			return (0);
		*cmd = *head;
		cursor->argc = 0;
	}
	return (1);
}

static int	handle_pipe_token(char **tokens, t_command **cmd,
	t_gc *gc, t_parser_cursor *cursor)
{
	if (tokens[cursor->i][0] == '|' && tokens[cursor->i][1] == '\0')
	{
		(*cmd)->argv[cursor->argc] = NULL;
		(*cmd)->next = new_command(gc);
		if (!(*cmd)->next)
			return (-1);
		*cmd = (*cmd)->next;
		cursor->argc = 0;
		(cursor->i)++;
		return (1);
	}
	return (0);
}

static int	append_token_to_argv(t_command *cmd, char *token,
	t_gc *gc, t_parser_cursor *cursor)
{
	size_t	j;
	char	**new_argv;

	new_argv = gc_malloc(gc, sizeof(char *) * ((cursor->argc) + 2));
	if (!new_argv)
		return (0);
	j = 0;
	while (j < (cursor->argc))
	{
		new_argv[j] = cmd->argv[j];
		j++;
	}
	new_argv[(cursor->argc)] = token;
	new_argv[(cursor->argc) + 1] = NULL;
	cmd->argv = new_argv;
	((cursor->argc))++;
	return (1);
}
