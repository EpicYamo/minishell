/* ************************************************************************************** */
/*                                                                                        */
/*                                                                   :::      ::::::::    */
/*   parser_pt_two.c                                               :+:      :+:    :+:    */
/*                                                               +:+ +:+         +:+      */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr>              +#+  +:+       +#+         */
/*                                                           +#+#+#+#+#+   +#+            */
/*   Created: 2025/06/16 01:58:00 by aaycan                       #+#    #+#              */
/*   Updated: 2025/06/16 01:58:00 by aaycan                      ###   ########.tr        */
/*                                                                                        */
/* ************************************************************************************** */

#include "minishell.h"
#include <stdio.h>

t_command	*new_command(t_gc *gc)
{
	t_command *cmd;

	cmd = gc_malloc(gc, sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->argv = gc_malloc(gc, (sizeof(char *) * 1));
	if (!cmd->argv)
		return (NULL);
	cmd->argv[0] = NULL;
	cmd->infile = NULL;
	cmd->outfile = NULL;
	cmd->append = 0;
	cmd->heredoc = 0;
	cmd->next = NULL;
	return (cmd);
}

int	handle_redirection_token(char **tokens, size_t *i, t_command *cmd)
{
	if ((tokens[*i][0] == '<' || tokens[*i][0] == '>') &&
		(tokens[*i + 1] == NULL || tokens[*i + 1][0] == '<' ||
		tokens[*i + 1][0] == '>' || tokens[*i + 1][0] == '|'))
	{
		printf("\033[0;31mSyntax error: near unexpected token `%s'\033[0m\n", tokens[*i]);
		return (-1);
	}
	if (tokens[*i][0] == '<' && tokens[*i][1] == '\0')
		cmd->infile = tokens[++(*i)], cmd->heredoc = 0;
	else if (tokens[*i][0] == '>' && tokens[*i][1] == '\0')
		cmd->outfile = tokens[++(*i)], cmd->append = 0;
	else if (tokens[*i][0] == '<' && tokens[*i][1] == '<' && tokens[*i][2] == '\0')
		cmd->infile = tokens[++(*i)], cmd->heredoc = 1;
	else if (tokens[*i][0] == '>' && tokens[*i][1] == '>' && tokens[*i][2] == '\0')
		cmd->outfile = tokens[++(*i)], cmd->append = 1;
	else
		return (0);
	return (1);
}
