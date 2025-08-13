/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_pt_eight.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 15:37:17 by aaycan            #+#    #+#             */
/*   Updated: 2025/08/14 00:20:18 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

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

	i = 0;
	while (i < io.proc_count)
	{
		if (waitpid(io.pids[i], io.exit_stat_ptr, 0) == -1)
		{
			perror("waitpid");
			(*io.exit_stat_ptr) = 2;
		}
		else if (WIFEXITED(*io.exit_stat_ptr))
			(*io.exit_stat_ptr) = WEXITSTATUS(*io.exit_stat_ptr);
		i++;
	}
	if ((*io.exit_stat_ptr) == 131)
		write(2, "Quit (core dumped)\n", 19);
	if ((*io.exit_stat_ptr) == 2)
	{
		(*io.exit_stat_ptr) = 130;
		write(1, "\n", 1);
	}
}
