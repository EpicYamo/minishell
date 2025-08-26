/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_pt_nine.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 04:59:57 by aaycan            #+#    #+#             */
/*   Updated: 2025/08/26 21:26:40 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <limits.h>
#include <stdlib.h>
#include <readline/readline.h>

static void	check_dollar_sign_pt_two(size_t *valid_sign, char *token,
				size_t *loc, size_t *last_sign);

size_t	check_dollar_sign_existance_heredoc(char *token, size_t *last_sign)
{
	size_t	i;
	size_t	valid_sign;
	size_t	loc;

	i = 0;
	valid_sign = 0;
	if (((*last_sign) != LLONG_MAX))
	{
		while (token[i])
		{
			if ((token[i] == '$') && (i == ((*last_sign) - 1)))
			{
				(*last_sign) = i;
				i = 0;
			}
			else
				i++;
		}
	}
	check_dollar_sign_pt_two(&valid_sign, token, &loc, last_sign);
	return (valid_sign);
}

static void	check_dollar_sign_pt_two(size_t *valid_sign, char *token,
	size_t *loc, size_t *last_sign)
{
	t_lexer_cursor	cursor;

	cursor.i = 0;
	cursor.loc = loc;
	while (token[(cursor.i)])
	{
		if ((token[(cursor.i)] == '$') && ((cursor.i) < (*last_sign)))
		{
			while (token[(cursor.i) + 1] && (token[(cursor.i) + 1] == '$'))
				(cursor.i)++;
			(*valid_sign) += 1;
			(*loc) = (cursor.i);
		}
		(cursor.i) += 1;
	}
	if (*valid_sign)
	{
		(*last_sign) = (*loc);
		(*valid_sign) = ((*loc) + 1);
	}
}

void	exit_command_in_child_proc(t_command *cmd, t_gc *gc,
	char **formatted_line, t_env *env_list)
{
	int	exit_code;
	int	arg_err_flag;

	exit_code = cmd->io->exit_status;
	arg_err_flag = 0;
	exit_command_validity(cmd, &arg_err_flag, &exit_code);
	if (exit_code != -1)
	{
		clean_before_exit(cmd, gc, formatted_line, env_list);
		exit(exit_code);
	}
	(*cmd->io->exit_stat_ptr) = 1;
}

void	clean_before_exit(t_command *cmd, t_gc *gc,
	char **formatted_line, t_env *env_list)
{
	close_fds_for_child_proc(cmd);
	rl_clear_history();
	gc_collect_all(gc);
	free_string_array(formatted_line);
	free_env_list(env_list);
}
