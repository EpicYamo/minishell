/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_pt_seven.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 02:20:01 by aaycan            #+#    #+#             */
/*   Updated: 2025/08/08 15:27:38 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <limits.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

static int	modify_apply_heredoc_file_pt_two(t_command *cmd, char *infile,
				t_gc *gc);
static int	apply_changes_to_heredoc_file(t_command *cmd, char *infile,
				t_gc *gc);

int	modify_apply_heredoc_file(t_command *cmd, char *infile, t_env *env_list)
{
	size_t	sign_loc;
	size_t	last_sign;
	size_t	prev_sign;
	t_gc	*gc;

	gc = init_garbage_collector(infile);
	if (!gc)
		return (1);
	gc_add(gc, infile);
	last_sign = LONG_MAX;
	prev_sign = last_sign;
	sign_loc = check_dollar_sign_existance(infile, &last_sign);
	while (sign_loc)
	{
		infile = expand_env_vars_if_applicable(infile,
				(sign_loc - 1), env_list, prev_sign);
		if (!infile)
		{
			gc_collect_all(gc);
			return (1);
		}
		gc_add(gc, infile);
		prev_sign = last_sign;
		sign_loc = check_dollar_sign_existance(infile, &last_sign);
	}
	if (modify_apply_heredoc_file_pt_two(cmd, infile, gc) != 0)
		return (1);
	return (0);
}

static int	modify_apply_heredoc_file_pt_two(t_command *cmd, char *infile,
	t_gc *gc)
{
	infile = replace_dollar_signs(infile, gc);
	if (!infile)
	{
		gc_collect_all(gc);
		return (1);
	}
	if (check_status_sign_existance(infile) != 0)
	{
		if (apply_status_sign(&(infile), cmd, gc) != 0)
		{
			gc_collect_all(gc);
			return (1);
		}
	}
	if (apply_changes_to_heredoc_file(cmd, infile, gc) != 0)
		return (1);
	return (0);
}

static int	apply_changes_to_heredoc_file(t_command *cmd, char *infile,
	t_gc *gc)
{
	int	fd;

	fd = open(cmd->infile, O_WRONLY | O_TRUNC, 0644);
	if (fd == -1)
	{
		gc_collect_all(gc);
		return (1);
	}
	write(fd, infile, ft_strlen(infile));
	gc_collect_all(gc);
	return (0);
}

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
