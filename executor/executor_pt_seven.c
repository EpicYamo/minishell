/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_pt_seven.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 02:20:01 by aaycan            #+#    #+#             */
/*   Updated: 2025/08/08 15:42:29 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <limits.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

static int	modify_apply_heredoc_file_pt_two(t_command *cmd, char *infile,
				t_gc *gc);
static int	apply_changes_to_heredoc_file(t_command *cmd, char *infile,
				t_gc *gc);
static int	init_vars(t_gc **gc, char *infile, size_t *last_sign,
				size_t *prev_sign);

int	modify_apply_heredoc_file(t_command *cmd, char *infile, t_env *env_list)
{
	size_t	sign_loc;
	size_t	last_sign;
	size_t	prev_sign;
	t_gc	*gc;

	if (init_vars(&gc, infile, &last_sign, &prev_sign) != 0)
		return (1);
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

static int	init_vars(t_gc **gc, char *infile, size_t *last_sign,
	size_t *prev_sign)
{
	(*gc) = init_garbage_collector(infile);
	if (!(*gc))
	{
		free(infile);
		return (1);
	}
	gc_add((*gc), infile);
	(*last_sign) = LONG_MAX;
	(*prev_sign) = (*last_sign);
	return (0);
}
