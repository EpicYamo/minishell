/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_pt_two.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 19:48:24 by aaycan            #+#    #+#             */
/*   Updated: 2025/07/22 17:01:44 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <readline/readline.h>

static void	execute_echo_outfile(t_command *cmd, int i, int newline);
static void	execute_pwd_outfile(t_command *cmd, char *output_line);

void	echo_command(t_command *cmd)
{
	int	i;
	int	newline;

	i = 1;
	newline = 1;
	if (cmd->argv[1] && (ft_strcmp(cmd->argv[1], "-n") == 0))
	{
		newline = 0;
		i++;
	}
	if (cmd->outfile)
		execute_echo_outfile(cmd, i, newline);
	else
	{
		while (cmd->argv[i])
		{
			printf("%s", cmd->argv[i]);
			if (cmd->argv[i + 1])
				printf(" ");
			i++;
		}
		if (newline)
			printf("\n");
	}
}

static void	execute_echo_outfile(t_command *cmd, int i, int newline)
{
	int	fd;

	if (cmd->append == 0)
		fd = open(cmd->outfile, O_WRONLY, 0644);
	else
		fd = open(cmd->outfile, O_WRONLY | O_APPEND, 0644);
	if (fd == -1)
	{
		perror("Error opening file");
		return ;
	}
	while (cmd->argv[i])
	{
		write(fd, cmd->argv[i], ft_strlen(cmd->argv[i]));
		if (cmd->argv[i + 1])
			write(fd, " ", 1);
		i++;
	}
	if (newline)
		write(fd, "\n", 1);
	close(fd);
}

void	pwd_command(t_command *cmd)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
		perror(cwd);
	else
	{
		if (cmd->outfile)
			execute_pwd_outfile(cmd, cwd);
		else
			printf("%s\n", cwd);
		free(cwd);
	}
}

static void	execute_pwd_outfile(t_command *cmd, char *output_line)
{
	int	fd;

	if (cmd->append == 0)
		fd = open(cmd->outfile, O_WRONLY, 0644);
	else
		fd = open(cmd->outfile, O_WRONLY | O_APPEND, 0644);
	if (fd == -1)
	{
		perror("Error opening file");
		return ;
	}
	write(fd, output_line, ft_strlen(output_line));
	write(fd, "\n", 1);
	close(fd);
}
