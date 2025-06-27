/* ************************************************************************************** */
/*                                                                                        */
/*                                                                   :::      ::::::::    */
/*   parser_pt_one.c                                               :+:      :+:    :+:    */
/*                                                               +:+ +:+         +:+      */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr>              +#+  +:+       +#+         */
/*                                                           +#+#+#+#+#+   +#+            */
/*   Created: 2025/06/16 01:58:05 by aaycan                       #+#    #+#              */
/*   Updated: 2025/06/16 01:58:05 by aaycan                      ###   ########.tr        */
/*                                                                                        */
/* ************************************************************************************** */

#include "minishell.h"
#include <stdlib.h>
#include <stdio.h>

static void	init_variables(size_t *i, size_t *argc, t_command **head, t_command **cmd);
static int	initialize_command_if_needed(t_command **cmd, t_command **head, size_t *argc, t_gc *gc);
static int	handle_pipe_token(char **tokens, size_t *i, t_command **cmd, size_t *argc, t_gc *gc);
static int	append_token_to_argv(t_command *cmd, char *token, size_t *argc, t_gc *gc);

t_command	*parse_tokens(char **tokens, t_gc *gc)
{
	t_command	*head;
	t_command	*cmd;
	size_t		i;
	size_t		argc;
	int			status;

	init_variables(&i, &argc, &cmd, &head);
	while (tokens[i])
	{
		if (!initialize_command_if_needed(&cmd, &head, &argc, gc))
			return (NULL);
		status = handle_pipe_token(tokens, &i, &cmd, &argc, gc);
		if (status == 1)
			continue;
		else if (status == -1)
			return (NULL);
		status = handle_redirection_token(tokens, &i, cmd);
		if (status == -1)
			return (NULL);
		else if (status == 1)
		{
			i++;
			continue;
		}
		if (!append_token_to_argv(cmd, tokens[i], &argc, gc))
			return (NULL);
		i++;
	}
	return (head);
}

static void	init_variables(size_t *i, size_t *argc, t_command **head, t_command **cmd)
{
	*i = 0;
	*argc = 0;
	*head = NULL;
	*cmd = NULL;
}

static int	initialize_command_if_needed(t_command **cmd, t_command **head, size_t *argc, t_gc *gc)
{
	if (!(*cmd) && !(*head))
	{
		*head = new_command(gc);
		if (!(*head))
			return (0);
		*cmd = *head;
		*argc = 0;
	}
	return (1);
}

static int	handle_pipe_token(char **tokens, size_t *i, t_command **cmd, size_t *argc, t_gc *gc)
{
	if (tokens[*i][0] == '|' && tokens[*i][1] == '\0')
	{
		(*cmd)->argv[*argc] = NULL;
		(*cmd)->next = new_command(gc);
		if (!(*cmd)->next)
			return (-1);
		*cmd = (*cmd)->next;
		*argc = 0;
		(*i)++;
		return (1);
	}
	return (0);
}

static int	append_token_to_argv(t_command *cmd, char *token, size_t *argc, t_gc *gc)
{
	size_t	j;
	char	**new_argv;

	new_argv = gc_malloc(gc, sizeof(char *) * (*argc + 2));
	if (!new_argv)
		return (0);
	j = 0;
	while (j < *argc)
	{
		new_argv[j] = cmd->argv[j];
		j++;
	}
	new_argv[*argc] = token;
	new_argv[*argc + 1] = NULL;
	cmd->argv = new_argv;
	(*argc)++;
	return (1);
}
