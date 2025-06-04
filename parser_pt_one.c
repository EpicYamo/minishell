/* ************************************************************************************** */
/*                                                                                        */
/*                                                                   :::      ::::::::    */
/*   parser_pt_one.c                                               :+:      :+:    :+:    */
/*                                                               +:+ +:+         +:+      */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr>              +#+  +:+       +#+         */
/*                                                           +#+#+#+#+#+   +#+            */
/*   Created: 2025/06/04 18:27:47 by aaycan                       #+#    #+#              */
/*   Updated: 2025/06/04 18:27:47 by aaycan                      ###   ########.tr        */
/*                                                                                        */
/* ************************************************************************************** */

#include "minishell.h"
#include <stdlib.h>
#include <stdio.h>

static t_command	*new_command(void);

t_command *parse_tokens(char **tokens)
{
	t_command	*head;
	t_command	*cmd;
	char		**new_argv;
	size_t		i;
	size_t		j;
	size_t		argc;

	cmd = NULL;
	head = NULL;
	i = 0;
	while (tokens[i])
	{
		if (!cmd && !head)
		{
			head = new_command();
			cmd = head;
			argc = 0;
		}
		if ((tokens[i][0] == '|') && (tokens[i][1] == '\0'))
		{
			free(tokens[i]);
			cmd->argv[argc] = NULL;
			cmd->next = new_command();
			cmd = cmd->next;
			argc = 0;
			i++;
			continue;
		}
		else if ((tokens[i][0] == '<' || tokens[i][0] == '>')
				&& ((tokens[i + 1] == NULL) || (tokens[i + 1][0] == '<')
				|| (tokens[i + 1][0] == '>') || (tokens[i + 1][0] == '|')))
		{
			printf("\033[0;31mSyntax error: near unexpected token `%s'\033[0m\n", tokens[i]);
			return (NULL); //first free everything else
		}
		else if (tokens[i][0] == '<' && tokens[i][1] == '<' && tokens[i][2] == '\0')
		{
			free(tokens[i]);
			if (cmd->infile)
				free(cmd->infile);
			cmd->infile = tokens[++i];
			cmd->heredoc = 1;
		}
		else if (tokens[i][0] == '<' && tokens[i][1] == '\0')
		{
			free(tokens[i]);
			if (cmd->infile)
				free(cmd->infile);
			cmd->infile = tokens[++i];
			cmd->heredoc = 0;
		}
		else if (tokens[i][0] == '>' && tokens[i][1] == '>' && tokens[i][2] == '\0')
		{
			free(tokens[i]);
			if (cmd->outfile)
				free(cmd->outfile);
			cmd->outfile = tokens[++i];
			cmd->append = 1;
		}
		else if (tokens[i][0] == '>' && tokens[i][1] == '\0')
		{
			free(tokens[i]);
			if (cmd->outfile)
				free(cmd->outfile);
			cmd->outfile = tokens[++i];
			cmd->append = 0;
		}
		else
		{
			new_argv = malloc(sizeof(char *) * (argc + 2));
			if (!new_argv)
				return (NULL); //first free the memory starting from the head command
			j = -1;
			while (++j < argc)
				new_argv[j] = cmd->argv[j];
			new_argv[argc++] = tokens[i];
			new_argv[argc] = NULL;
			free(cmd->argv);
			cmd->argv = new_argv;
			new_argv = NULL;
		}
		i++;
	}
	return (head);
}

static t_command	*new_command(void)
{
	t_command *cmd;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->argv = malloc(sizeof(char *) * 1);
	if (!cmd->argv)
	{
		free(cmd);
		return (NULL);
	}
	cmd->argv[0] = NULL;
	cmd->infile = NULL;
	cmd->outfile = NULL;
	cmd->append = 0;
	cmd->heredoc = 0;
	cmd->next = NULL;
	return (cmd);
}
