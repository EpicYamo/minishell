/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_pt_one.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 03:26:26 by aaycan            #+#    #+#             */
/*   Updated: 2025/07/12 23:11:38 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

void	command_executor(t_command *cmd, t_gc *gc, char	**formatted_line,
				t_env *env_list)
{
	while (cmd)
	{
		if (!cmd->argv || !cmd->argv[0])
		{
			cmd = cmd->next;
			continue ;
		}
		if (is_builtin(cmd->argv[0]))
			execute_built_in_commands(cmd, gc, formatted_line, env_list);
		else
			execute_command();
		if (cmd->heredoc == 1)
			unlink(cmd->infile);
		cmd = cmd->next;
	}
}

static void	execute_command()
{
	
}
