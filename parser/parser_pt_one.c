/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_pt_one.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 02:34:55 by aaycan            #+#    #+#             */
/*   Updated: 2025/08/29 17:17:59 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	init_variables(t_parser_cursor *cursor, t_command **head,
				t_command **cmd, t_io *io);
static int	initialize_command_if_needed(t_command **cmd, t_command **head,
				t_parser_cursor *cursor, t_gc *gc);
static int	parse_tokens_to_be_interpreted(t_interpret_data_tokens data,
				t_gc *gc, t_command **cmd, t_parser_cursor *cursor);
static void	combine_tokens_interpret_data(t_interpret_data_tokens *data,
				char **tokens, t_interpret interpret_set);

t_command	*parse_tokens(char **tokens, t_gc *gc, t_io *io,
	t_interpret interpret_set)
{
	t_command				*head;
	t_command				*cmd;
	t_parser_cursor			cursor;
	int						status;
	t_interpret_data_tokens	data;

	init_variables(&cursor, &cmd, &head, io);
	combine_tokens_interpret_data(&data, tokens, interpret_set);
	while (tokens[cursor.i])
	{
		if (!initialize_command_if_needed(&cmd, &head, &cursor, gc))
			return (NULL);
		if (interpret_set.flag_set[cursor.i] == 0)
		{
			status = parse_tokens_to_be_interpreted(data, gc, &cmd, &cursor);
			if (status == 0)
				continue ;
			else if (status == -1)
				return (NULL);
		}
		if (!append_token_to_argv(cmd, tokens[(cursor.i)], gc, &cursor))
			return (NULL);
		(cursor.i)++;
	}
	return (head);
}

static void	init_variables(t_parser_cursor *cursor,
	t_command **head, t_command **cmd, t_io *io)
{
	cursor->i = 0;
	cursor->argc = 0;
	cursor->io = io;
	(*cursor->io->exit_stat_ptr) = 0;
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

static int	parse_tokens_to_be_interpreted(t_interpret_data_tokens data,
	t_gc *gc, t_command **cmd, t_parser_cursor *cursor)
{
	int			status;
	char		**tokens;
	t_interpret	interpret_set;
	t_garbages	all_garbage;

	all_garbage.gc = gc;
	all_garbage.formatted_line = data.interpret_set.formatted_line;
	all_garbage.env_list = data.interpret_set.env_list;
	tokens = data.tokens;
	interpret_set = data.interpret_set;
	status = handle_pipe_token(tokens, cmd, gc, cursor);
	if (status == 1)
		return (0);
	else if (status == -1)
		return (-1);
	if ((!(ft_strcmp(tokens[(*cursor).i], "<<")))
		&& ((*cursor).i < (interpret_set.token_count - 1))
		&& (interpret_set.flag_set[(*cursor).i + 1] == 1))
		(*cmd)->expand_heredoc = 0;
	status = handle_redirection_token(tokens, (*cmd), all_garbage, cursor);
	if (status == -1)
		return (-1);
	else if (status == 1)
		return (0);
	return (1);
}

static void	combine_tokens_interpret_data(t_interpret_data_tokens *data,
	char **tokens, t_interpret interpret_set)
{
	(*data).tokens = tokens;
	(*data).interpret_set = interpret_set;
}
