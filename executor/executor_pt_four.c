/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_pt_four.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 15:40:35 by aaycan            #+#    #+#             */
/*   Updated: 2025/08/01 02:39:52 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void	setup_infile_redirect(t_command *cmd, t_env *env_list)
{
	int	fd;

	if (cmd->infile)
	{
		if (cmd->heredoc == 1)
		{
			if (expand_dollar_sign_in_heredoc(cmd, env_list) != 0)
			{
				perror("infile");
				exit(127);
			}
		}
		fd = open(cmd->infile, O_RDONLY);
		if (fd == -1)
		{
			perror("infile");
			exit(127);
		}
		dup2(fd, STDIN_FILENO);
		close(fd);
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
		{
			perror("outfile");
			exit(127);
		}
		else
		{
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
	}
}
