/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_pt_eight.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 15:37:17 by aaycan            #+#    #+#             */
/*   Updated: 2025/08/26 20:55:50 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <dirent.h>
#include <fcntl.h>

static int	test_fd_and_folder(t_command *cmd, DIR *folder, int fd);

int	skip_command(t_command **cmd)
{
	if (((*cmd)->argv == NULL) || ((*cmd)->argv[0] == NULL))
	{
		(*cmd) = (*cmd)->next;
		return (1);
	}
	return (0);
}

int	apply_pipe(t_io *io)
{
	if (pipe((*io).pipe_fd) == -1)
	{
		perror("pipe");
		return (1);
	}
	return (0);
}

void	process_child_processes(t_io io)
{
	int	i;
	int	flag;

	i = 0;
	flag = 0;
	while (i < io.proc_count)
	{
		if (waitpid(io.pids[i], io.exit_stat_ptr, 0) == -1)
		{
			perror("waitpid");
			(*io.exit_stat_ptr) = 2;
		}
		else if (WIFEXITED(*io.exit_stat_ptr))
			(*io.exit_stat_ptr) = WEXITSTATUS(*io.exit_stat_ptr);
		else if (WIFSIGNALED(*io.exit_stat_ptr))
		{
			(*io.exit_stat_ptr) = 128 + WTERMSIG(*io.exit_stat_ptr);
			if (*io.exit_stat_ptr == 130 && flag == 0)
				put_newline_and_set_flag(&flag);
		}
		i++;
	}
	if ((*io.exit_stat_ptr) == 131)
		write(2, "Quit (core dumped)\n", 19);
}

int	test_path(t_command *cmd)
{
	DIR	*folder;
	int	fd;
	int	err;

	if (ft_strchr(cmd->argv[0], '/') == NULL)
		return (0);
	if (if_executeable(cmd) == 1)
		return (0);
	fd = open(cmd->argv[0], O_WRONLY);
	folder = opendir(cmd->argv[0]);
	err = test_fd_and_folder(cmd, folder, fd);
	if (err == 1)
		(*cmd->io->exit_stat_ptr) = 126;
	if (folder)
		closedir(folder);
	if (fd != -1)
		close(fd);
	return (err);
}

static int	test_fd_and_folder(t_command *cmd, DIR *folder, int fd)
{
	if (fd == -1 && folder == NULL)
	{
		write_error_with_arg(cmd);
		write(2, ": No such file or directory\n", 28);
		return (1);
	}
	else if (fd == -1 && folder != NULL)
	{
		write_error_with_arg(cmd);
		write(2, ": is a directory\n", 17);
		return (1);
	}
	else if (fd != -1 && folder == NULL)
	{
		write_error_with_arg(cmd);
		write(2, ": Permission denied\n", 20);
		return (1);
	}
	return (0);
}
