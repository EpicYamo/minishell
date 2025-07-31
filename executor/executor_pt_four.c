/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_pt_four.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 15:40:35 by aaycan            #+#    #+#             */
/*   Updated: 2025/07/31 15:46:33 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

void	setup_infile_redirect(t_command *cmd)
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

void	setup_outfile_redirect(t_command *cmd)
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
