/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_pt_three.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 17:21:42 by aaycan            #+#    #+#             */
/*   Updated: 2025/07/30 17:51:07 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

static void	exec_non_built_in_com_in_child_proc(t_command *cmd,
				t_env *env_list);
static void	setup_infile_redirect(t_command *cmd);
static void	setup_outfile_redirect(t_command *cmd);

void	execute_non_built_in_command(t_command *cmd, t_env *env_list)
{
	pid_t	proc_pid;

	proc_pid = fork();
	if (proc_pid == 0)
		exec_non_built_in_com_in_child_proc(cmd, env_list);
	if (proc_pid < 0)
	{
		perror("fork");
		return ;
	}
	if (waitpid(proc_pid, &cmd->io->exit_status, 0) == -1)
	{
		perror("waitpid");
		return ;
	}
	if (cmd->io->prev_fd != -1)
		close(cmd->io->prev_fd);
	if (cmd->next != NULL)
		cmd->io->prev_fd = cmd->io->pipe_fd[0];
	else
		close(cmd->io->pipe_fd[0]);
	close(cmd->io->pipe_fd[1]);
}

static void	exec_non_built_in_com_in_child_proc(t_command *cmd, t_env *env_list)
{
	char	*path;

	signal(SIGINT, handle_sigint);
	if (cmd->next)
		dup2(cmd->io->pipe_fd[1], STDOUT_FILENO);
	close(cmd->io->pipe_fd[1]);
	close(cmd->io->pipe_fd[0]);
	if (cmd->io->prev_fd != -1)
	{
		dup2(cmd->io->prev_fd, STDIN_FILENO);
		close(cmd->io->prev_fd);
	}
	setup_infile_redirect(cmd);
	setup_outfile_redirect(cmd);
	path = resolve_path(cmd->argv[0], env_list);
	if (execve(path, cmd->argv, get_envp(env_list)) == -1)
	{
		perror("execve");
		exit(127);
	}
}

static void	setup_infile_redirect(t_command *cmd)
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
}

static void	setup_outfile_redirect(t_command *cmd)
{
	int	fd;

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
