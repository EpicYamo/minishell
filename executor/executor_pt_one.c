/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_pt_one.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 03:26:26 by aaycan            #+#    #+#             */
/*   Updated: 2025/07/30 17:51:19 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

static void	init_io(t_io *io);
static int	skip_command(t_command **cmd);
static int	apply_pipe(t_io *io);
static void	return_to_original_state(t_io io);

void	command_executor(t_command *cmd, t_gc *gc, char **formatted_line,
	t_env *env_list)
{
	t_io	io;

	init_io(&io);
	if ((cmd) && (cmd->next == NULL) && (is_builtin(cmd->argv[0])))
	{
		execute_built_in_commands(cmd, gc, formatted_line, env_list);
		cmd = cmd->next;
	}
	signal(SIGINT, SIG_IGN);
	while (cmd)
	{
		cmd->io = &io;
		if (skip_command(&cmd) != 0)
			continue ;
		if (apply_pipe(&io) != 0)
			break ;
		if (is_builtin(cmd->argv[0]))
			exec_built_in_com_in_child_proc(cmd, gc, formatted_line, env_list);
		else
			execute_non_built_in_command(cmd, env_list);
		if (cmd->heredoc == 1)
			unlink(cmd->infile);
		cmd = cmd->next;
	}
	return_to_original_state(io);
}

static void	init_io(t_io *io)
{
	(*io).prev_fd = -1;
	(*io).original_stdin = dup(STDIN_FILENO);
	(*io).original_stdout = dup(STDOUT_FILENO);
}

static int	skip_command(t_command **cmd)
{
	if (((*cmd)->argv == NULL) || ((*cmd)->argv[0] == NULL))
	{
		(*cmd) = (*cmd)->next;
		return (1);
	}
	return (0);
}

static int	apply_pipe(t_io *io)
{
	if (pipe((*io).pipe_fd) == -1)
	{
		perror("pipe");
		return (1);
	}
	return (0);
}

static void	return_to_original_state(t_io io)
{
	dup2(io.original_stdin, STDIN_FILENO);
	dup2(io.original_stdout, STDOUT_FILENO);
	close(io.original_stdin);
	close(io.original_stdout);
	signal(SIGINT, handle_sigint);
}
