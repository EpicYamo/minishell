/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_pt_three.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 13:09:45 by aaycan            #+#    #+#             */
/*   Updated: 2025/06/28 13:20:57 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>

void	exit_command(t_command *cmd, t_gc *gc, char *line)
{
	int	exit_code;
	int	i;
	int	arg_err_flag;

	exit_code = 0;
	arg_err_flag = 0;
	printf("exit\n");
	if (cmd->argv[1])
	{
		exit_code = ft_atoi(cmd->argv[1]);
		i = -1;
		while (cmd->argv[1][++i])
		{
			if (ft_isdigit(cmd->argv[1][i]) == 0)
			{
				printf("Y-Shell: exit: %s: numeric argument required\n", cmd->argv[1]);
				arg_err_flag = 1;
				exit_code = 2;
				break;
			}
		}
	}
	if ((arg_err_flag == 0) && (cmd->argv[1] != NULL) && (cmd->argv[2] != NULL))
	{
		printf("Y-Shell: exit: too many arguments\n");
		exit_code = 1;
	}
	rl_clear_history();
	gc_collect_all(gc);
	free(line);
	exit(exit_code);
}
