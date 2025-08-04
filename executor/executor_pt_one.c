/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_pt_one.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 03:26:26 by aaycan            #+#    #+#             */
/*   Updated: 2025/08/04 18:20:16 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>

static int	init_io(t_io *io, t_command *cmd, t_gc *gc, int *exit_status);
static void	init_io_stack_vars(t_io *io, t_command *cmd, int *i);
static void	return_to_original_state(t_io io, int *exit_status);

void	command_executor(t_command *cmd, t_gc *gc, char **formatted_line,
	t_env *env_list)
{
	static int	exit_status = 0;
	t_io		io;

	if (init_io(&io, cmd, gc, &exit_status) != 0)
		return ;
	if (execute_single_built_in_command(&cmd, gc,
			formatted_line, env_list) != 0)
		return ;
	signal(SIGINT, SIG_IGN);
	while (cmd)
	{
		cmd->io = &io;
		if (apply_exit_status_token(cmd, gc) != 0)
			break ;
		if (skip_command(&cmd) != 0)
			continue ;
		if (apply_pipe(&io) != 0)
			break ;
		if (is_builtin(cmd->argv[0]))
			exec_built_in_com_in_child_proc(cmd, gc, formatted_line, env_list);
		else
			execute_non_built_in_command(cmd, env_list);
		cmd = cmd->next;
	}
	return_to_original_state(io, &exit_status);
}

static int	init_io(t_io *io, t_command *cmd, t_gc *gc, int *exit_status)
{
	int	*pids_arr;
	int	i;

	(*io).exit_status = (*exit_status);
	(*io).exit_stat_ptr = exit_status;
	init_io_stack_vars(io, cmd, &i);
	pids_arr = gc_malloc(gc, (sizeof(int) * i));
	if (!pids_arr)
	{
		perror("malloc");
		return (1);
	}
	(*io).pids = pids_arr;
	return (0);
}

static void	init_io_stack_vars(t_io *io, t_command *cmd, int *i)
{
	(*io).prev_fd = -1;
	(*io).original_stdin = dup(STDIN_FILENO);
	(*io).original_stdout = dup(STDOUT_FILENO);
	if (cmd)
		cmd->io = io;
	(*io).heredoc_path = NULL;
	(*io).proc_count = 0;
	(*i) = 0;
	while (cmd)
	{
		(*i)++;
		if (cmd->heredoc == 1)
			(*io).heredoc_path = cmd->infile;
		cmd = cmd->next;
	}
}

static void	return_to_original_state(t_io io, int *exit_status)
{
	int	i;

	i = 0;
	while (i < io.proc_count)
	{
		if (waitpid(io.pids[i], exit_status, 0) == -1)
			perror("waitpid");
		else if (WIFEXITED(*exit_status))
			(*exit_status) = WEXITSTATUS(*exit_status);
		i++;
	}
	dup2(io.original_stdin, STDIN_FILENO);
	dup2(io.original_stdout, STDOUT_FILENO);
	close(io.original_stdin);
	close(io.original_stdout);
	if (io.prev_fd != -1)
		close(io.prev_fd);
	signal(SIGINT, handle_sigint);
	if (io.heredoc_path != NULL)
		unlink(io.heredoc_path);
}
