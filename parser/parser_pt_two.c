/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_pt_two.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 02:34:51 by aaycan            #+#    #+#             */
/*   Updated: 2025/07/09 21:49:13 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

static int	validate_redirection_syntax(char **tokens, size_t *i);
static int	validate_redirection_file(const char *filename, int type);
static int	assign_redirection(t_command *cmd, char **tokens,
				size_t *i, int type);

t_command	*new_command(t_gc *gc)
{
	t_command	*cmd;

	cmd = gc_malloc(gc, sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->argv = gc_malloc(gc, (sizeof(char *) * 1));
	if (!cmd->argv)
		return (NULL);
	cmd->argv[0] = NULL;
	cmd->infile = NULL;
	cmd->outfile = NULL;
	cmd->append = 0;
	cmd->heredoc = 0;
	cmd->next = NULL;
	return (cmd);
}

int	handle_redirection_token(char **tokens, t_command *cmd,
	t_gc *gc, t_parser_cursor *cursor)
{
	if (validate_redirection_syntax(tokens, &cursor->i) == -1)
		return (-1);
	if (ft_strcmp(tokens[(*cursor).i], "<") == 0)
		return (assign_redirection(cmd, tokens, &cursor->i, 0));
	else if (ft_strcmp(tokens[(*cursor).i], ">") == 0)
		return (assign_redirection(cmd, tokens, &cursor->i, 1));
	else if (ft_strcmp(tokens[(*cursor).i], ">>") == 0)
		return (assign_redirection(cmd, tokens, &cursor->i, 2));
	else if (ft_strcmp(tokens[(*cursor).i], "<<") == 0)
		return (handle_heredoc(cmd, tokens, &cursor->i, gc));
	return (0);
}

static int	validate_redirection_syntax(char **tokens, size_t *i)
{
	if ((ft_strcmp(tokens[*i], "<") == 0
			|| ft_strcmp(tokens[*i], ">") == 0
			|| ft_strcmp(tokens[*i], "<<") == 0
			|| ft_strcmp(tokens[*i], ">>") == 0))
	{
		if (tokens[*i + 1] == NULL)
		{
			printf("Y-Shell: syntax error near unexpected token `newline'\n");
			return (-1);
		}
		else if (ft_strcmp(tokens[*i + 1], "<") == 0
			|| ft_strcmp(tokens[*i + 1], ">") == 0
			|| ft_strcmp(tokens[*i + 1], "<<") == 0
			|| ft_strcmp(tokens[*i + 1], ">>") == 0
			|| ft_strcmp(tokens[*i + 1], "|") == 0)
		{
			printf("Y-Shell: syntax error near unexpected token `%s'\n",
				tokens[*i + 1]);
			return (-1);
		}
	}
	return (0);
}

static int	assign_redirection(t_command *cmd, char **tokens,
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

static int	validate_redirection_file(const char *filename, int type)
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
