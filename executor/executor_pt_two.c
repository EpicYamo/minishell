/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_pt_two.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 18:04:26 by aaycan            #+#    #+#             */
/*   Updated: 2025/07/28 22:28:42 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "executor.h"
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

static void	execute_command_in_child_process(t_command *cmd, t_env *env_list, t_io *io);
static void	setup_child_proc_redirects(t_command *cmd, t_io *io);

void	execute_non_built_in_command(t_command *cmd, t_env *env_list, t_io *io)
{
	pid_t	proc_pid;

	signal(SIGINT, SIG_IGN);
	proc_pid = fork();
	if (proc_pid == 0)
		execute_command_in_child_process(cmd, env_list, io);
	signal(SIGINT, handle_sigint);
	if (proc_pid < 0)
	{
		perror("fork");
		return ;
	}
	if (waitpid(proc_pid, &(*io).exit_status, 0) == -1)
	{
		perror("waitpid");
		return ;
	}
	if ((*io).prev_fd != -1)
		close((*io).prev_fd);
	if (cmd->next)
	{
		close((*io).pipe_fd[1]);
		(*io).prev_fd = (*io).pipe_fd[0];
	}
}

static void	execute_command_in_child_process(t_command *cmd, t_env *env_list, t_io *io)
{
	char	*path;

	signal(SIGINT, handle_sigint);
	setup_child_proc_redirects(cmd, io);
	if (io->prev_fd != -1)
	{
		dup2(io->prev_fd, STDIN_FILENO);
		close(io->prev_fd);
	}
	if (cmd->next)
	{
		close(io->pipe_fd[0]);
		dup2(io->pipe_fd[1], STDOUT_FILENO);
		close(io->pipe_fd[1]);
	}
	path = resolve_path(cmd->argv[0], env_list);
	if (execve(path, cmd->argv, get_envp(env_list)) == -1)
	{
		perror("execve");
		exit(127);
	}
}

static void	setup_child_proc_redirects(t_command *cmd, t_io *io)
{
	int	fd;

	if (cmd->infile)
	{
		fd = open(cmd->infile, O_RDONLY);
		if (fd == -1)
			perror("infile");
		else
		{
			dup2(fd, STDIN_FILENO);
			close(fd);
		}
	}
	else if (io->built_in_io_flag == 1)
	{
		dup2((*io).pipe_fd[0], STDIN_FILENO);
		close((*io).pipe_fd[0]);
	}
	if (cmd->outfile)
	{
		if (cmd->append == 1)
			fd = open(cmd->outfile, O_WRONLY | O_APPEND, 0644);
		else
			fd = open(cmd->outfile, O_WRONLY, 0644);
		if (fd == -1)
			perror("outfile");
		else
		{
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
	}
}
