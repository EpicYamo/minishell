/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_pt_four.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 21:26:02 by aaycan            #+#    #+#             */
/*   Updated: 2025/08/04 21:26:45 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int	assign_redirection(t_command *cmd, char **tokens,
	size_t *i, int type)
{
	if (validate_redirection_file(tokens[*i + 1], type) == -1)
		return (-1);
	if (type == 0)
	{
		cmd->infile = tokens[++(*i)];
		cmd->heredoc = 0;
	}
	else
	{
		cmd->outfile = tokens[++(*i)];
		cmd->append = (type == 2);
	}
	(*i)++;
	return (1);
}

int	validate_redirection_file(const char *filename, int type)
{
	int	fd;

	if (type == 1)
		fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (type == 2)
		fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else if (type == 0)
		fd = open(filename, O_RDONLY);
	else
		return (-1);
	if (fd == -1)
	{
		perror(filename);
		return (-1);
	}
	close(fd);
	return (0);
}
