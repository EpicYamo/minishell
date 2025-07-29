/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_pt_one.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 03:26:26 by aaycan            #+#    #+#             */
/*   Updated: 2025/07/29 19:26:24 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "executor.h"
#include <unistd.h>
#include <stdio.h>

static int	skip_command(t_command *cmd);
static int	apply_pipe(t_io *io);
static void	setup_built_in_redirects(t_command *cmd, t_io *io);

void	command_executor(t_command *cmd, t_gc *gc, char **formatted_line,
				t_env *env_list)
{
	t_io	io;

	io.prev_fd = -1;
	io.built_in_io_flag = 0;
	io.original_stdout = dup(STDOUT_FILENO);
	io.original_stdout = dup(STDIN_FILENO);
	while (cmd)
	{
		if (skip_command(cmd) != 0)
			continue ;
		if (apply_pipe(&io) != 0)
			return ;
		if (is_builtin(cmd->argv[0]))
		{
			write(io.original_stdout, "reached\n", 8);
			setup_built_in_redirects(cmd, &io);
			execute_built_in_commands(cmd, gc, formatted_line, env_list);
		}
		else
			execute_non_built_in_command(cmd, env_list, &io);
		if (cmd->heredoc == 1)
			unlink(cmd->infile);
		cmd = cmd->next;
	}
}

static int	skip_command(t_command *cmd)
{
	if (!cmd->argv || !cmd->argv[0])
	{
		cmd = cmd->next;
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

static void	setup_built_in_redirects(t_command *cmd, t_io *io)
{
	if (cmd->next)
	{
		dup2((*io).pipe_fd[1], STDOUT_FILENO);
		(*io).built_in_io_flag = 1;
	}
	else
	{
		if ((*io).built_in_io_flag == 1)
		{
			dup2((*io).original_stdout, STDOUT_FILENO);
			(*io).built_in_io_flag = 0;
		}
	}	
}
