/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_pt_five.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 21:05:38 by aaycan            #+#    #+#             */
/*   Updated: 2025/08/18 16:38:54 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <unistd.h>

int	handle_pipe_token(char **tokens, t_command **cmd,
	t_gc *gc, t_parser_cursor *cursor)
{
	if ((tokens[cursor->i][0] == '|') && (tokens[cursor->i][1] == '\0'))
	{
		if ((tokens[(cursor->i) + 1] == NULL)
			|| ((tokens[(cursor->i) + 1][0] == '|')
			&& (tokens[(cursor->i) + 1][1] == '\0')))
		{
			write(2, "Y-Shell: syntax error near unexpected token `|'\n", 48);
			return (-1);
		}
	}
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

int	append_token_to_argv(t_command *cmd, char *token,
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
