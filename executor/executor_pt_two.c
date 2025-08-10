/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_pt_two.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 18:04:26 by aaycan            #+#    #+#             */
/*   Updated: 2025/08/10 21:15:00 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <signal.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

static void	io_prep(t_command *cmd);
static void	close_fds_for_child_proc(t_command *cmd);

void	exec_built_in_com_in_child_proc(t_command *cmd, t_gc *gc,
	char **formatted_line, t_env **env_list)
{
	pid_t	proc_pid;

	io_prep(cmd);
	proc_pid = fork();
	if (proc_pid == 0)
	{
		signal(SIGINT, handle_sigint_interactive);
		signal(SIGQUIT, SIG_DFL);
		if (cmd->next)
			dup2(cmd->io->pipe_fd[1], STDOUT_FILENO);
		close_fds_for_child_proc(cmd);
		execute_built_in_commands(cmd, gc, formatted_line, env_list);
		exit((*cmd->io->exit_stat_ptr));
	}
	if (proc_pid < 0)
	{
		perror("fork");
		return ;
	}
	cmd->io->pids[cmd->io->proc_count] = proc_pid;
	cmd->io->proc_count += 1;
	close(cmd->io->pipe_fd[1]);
}

static void	io_prep(t_command *cmd)
{
	if (cmd->io->prev_fd != -1)
		close(cmd->io->prev_fd);
	if (cmd->next != NULL)
		cmd->io->prev_fd = cmd->io->pipe_fd[0];
	else
		close(cmd->io->pipe_fd[0]);
}

static void	close_fds_for_child_proc(t_command *cmd)
{
	close(cmd->io->pipe_fd[1]);
	close(cmd->io->pipe_fd[0]);
	if (cmd->io->prev_fd != -1)
		close(cmd->io->prev_fd);
}

int	execute_single_built_in_command(t_command **cmd, t_gc *gc,
	char **formatted_line, t_env **env_list)
{
	if (skip_command(cmd) != 0)
		return (0);
	if ((*cmd) && ((*cmd)->next == NULL) && (is_builtin((*cmd)->argv[0])))
	{
		if (apply_exit_status_token((*cmd), gc) != 0)
			return (1);
		execute_built_in_commands((*cmd), gc, formatted_line, env_list);
		(*cmd) = (*cmd)->next;
	}
	return (0);
}
