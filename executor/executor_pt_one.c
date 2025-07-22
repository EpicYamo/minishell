/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_pt_one.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 03:26:26 by aaycan            #+#    #+#             */
/*   Updated: 2025/07/22 18:51:08 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/wait.h>
#include <fcntl.h>

typedef struct s_file_descriptors
{
	int	pipe_fd[2];
	int	prev_fd;
}	t_fds;

static void	setup_redirects(t_command *cmd);
static void	execute_command_in_child_process(t_command *cmd, t_gc *gc, t_env *env_list, t_fds *fds);

void	command_executor(t_command *cmd, t_gc *gc, char **formatted_line,
				t_env *env_list)
{
	t_fds	fds;
	pid_t	proc_pid;
	int		status;
	int		saved_stdout;

	fds.prev_fd = -1;
	while (cmd)
	{
		if (!cmd->argv || !cmd->argv[0])
		{
			cmd = cmd->next;
			continue ;
		}
		if (cmd->next && (pipe(fds.pipe_fd) == -1))
		{
			perror("pipe");
			return ;
		}
		if (is_builtin(cmd->argv[0]))
		{
			if (cmd->next)
			{
				saved_stdout = dup(STDOUT_FILENO);
				dup2(fds.pipe_fd[1], STDOUT_FILENO);
			}
			execute_built_in_commands(cmd, gc, formatted_line, env_list);
			dup2(saved_stdout, STDOUT_FILENO);
		}
		else
		{
			proc_pid = fork();
			if (proc_pid == 0)
				execute_command_in_child_process(cmd, gc, env_list, &fds);
			else if (proc_pid < 0)
			{
				perror("fork");
				return ;
			}
			if (waitpid(proc_pid, &status, 0) == -1)
			{
				perror("waitpid");
				return ;
			}
			if (fds.prev_fd != -1)
				close(fds.prev_fd);
			if (cmd->next)
			{
				close(fds.pipe_fd[1]);
				fds.prev_fd = fds.pipe_fd[0];
			}
		}
		if (cmd->heredoc == 1)
			unlink(cmd->infile);
		cmd = cmd->next;
	}
}

static void	setup_redirects(t_command *cmd)
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

static void	execute_command_in_child_process(t_command *cmd, t_gc *gc, t_env *env_list, t_fds *fds)
{
	char	*path;

	(void)gc;
	setup_redirects(cmd);
	if (fds->prev_fd != -1)
	{
		dup2(fds->prev_fd, STDIN_FILENO);
		close(fds->prev_fd);
	}
	if (cmd->next)
	{
		close(fds->pipe_fd[0]);
		dup2(fds->pipe_fd[1], STDOUT_FILENO);
		close(fds->pipe_fd[1]);
	}
	path = resolve_path(cmd->argv[0], env_list);
	if (execve(path, cmd->argv, get_envp(env_list)) == -1)
	{
		perror("execve");
		exit(127);
	}
}
