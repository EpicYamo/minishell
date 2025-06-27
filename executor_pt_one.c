/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_pt_one.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 19:09:48 by aaycan            #+#    #+#             */
/*   Updated: 2025/06/27 19:40:52 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

static int	is_builtin(const char *cmd);

void	command_executor(t_command *cmd, t_gc *gc, char *line)
{
	while (cmd)
	{
		if (!cmd->argv || !cmd->argv[0])
		{
			cmd = cmd->next;
			continue;
		}
		if (is_builtin(cmd->argv[0]))
			execute_built_in_commands(cmd, gc, line);
		cmd = cmd->next;
	}
}

static int	is_builtin(const char *cmd)
{
	int	flag;

	flag = ft_strcmp(cmd, "echo");
	if (flag == 0)
		return (1);
	flag = ft_strcmp(cmd, "cd");
	if (flag == 0)
		return (1);
	flag = ft_strcmp(cmd, "pwd");
	if (flag == 0)
		return (1);
	flag = ft_strcmp(cmd, "export");
	if (flag == 0)
		return (1);
	flag = ft_strcmp(cmd, "unset");
	if (flag == 0)
		return (1);
	flag = ft_strcmp(cmd, "env");
	if (flag == 0)
		return (1);
	flag = ft_strcmp(cmd, "exit");
	if (flag == 0)
		return (1);
	return (0);
}
