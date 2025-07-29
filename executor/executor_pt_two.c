/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_pt_two.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 18:04:26 by aaycan            #+#    #+#             */
/*   Updated: 2025/07/29 22:03:07 by aaycan           ###   ########.fr       */
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
static void	setup_child_proc_redirects(t_command *cmd);

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
	if (waitpid(proc_pid, &io->exit_status, 0) == -1)
	{
		perror("waitpid");
		return ;
	}
	if (((*io).input_redir_flag == 1) && (cmd->next == NULL))
		(*io).input_redir_flag = 0;
}

static void	execute_command_in_child_process(t_command *cmd, t_env *env_list, t_io *io)
{
	char	*path;

	signal(SIGINT, handle_sigint);
	if ((*io).input_redir_flag == 1)
		dup2((*io).pipe_fd[0], STDIN_FILENO);
	else
		dup2((*io).std_input, STDIN_FILENO);
	if (cmd->next)
		dup2(io->pipe_fd[1], STDOUT_FILENO);
	else
		dup2((*io).std_out, STDOUT_FILENO);
	setup_child_proc_redirects(cmd);
	path = resolve_path(cmd->argv[0], env_list);
	if (execve(path, cmd->argv, get_envp(env_list)) == -1)
	{
		perror("execve");
		exit(127);
	}
}

static void	setup_child_proc_redirects(t_command *cmd)
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
