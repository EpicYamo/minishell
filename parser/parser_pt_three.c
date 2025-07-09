/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_pt_three.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 02:34:45 by aaycan            #+#    #+#             */
/*   Updated: 2025/07/09 20:09:16 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "fcntl.h"
#include <unistd.h>
#include <errno.h>
#include <readline/readline.h>
#include <stdlib.h>

static void	handle_heredoc_pt_two(t_command *cmd, char *delimiter, int fd,
				char *tempfile);

int	handle_heredoc(t_command *cmd, char **tokens, size_t *i, t_gc *gc)
{
	char	*delimiter;
	char	*tempfile;
	int		fd;

	if ((cmd->infile != NULL) && (cmd->heredoc == 1))
		unlink(cmd->infile);
	delimiter = tokens[++(*i)];
	tempfile = ft_strdup("/tmp/tmpfileAXO135FJnT9AcnAJkKEALGKWKmZ.txt");
	if (!tempfile)
	{
		perror(tempfile);
		return (-1);
	}
	else
		gc_add(gc, tempfile);
	fd = open(tempfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror("Error: creating heredoc file");
		return (-1);
	}
	handle_heredoc_pt_two(cmd, delimiter, fd, tempfile);
	(*i)++;
	return (1);
}

static void	handle_heredoc_pt_two(t_command *cmd, char *delimiter, int fd,
	char *tempfile)
{
	char	*line;

	line = readline("> ");
	while (line != NULL)
	{
		if ((ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0)
			&& (line[ft_strlen(delimiter)] == '\0'))
		{
			free(line);
			break ;
		}
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
		line = readline("> ");
	}
	close(fd);
	cmd->infile = tempfile;
	cmd->heredoc = 1;
}
