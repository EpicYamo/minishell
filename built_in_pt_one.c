/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_pt_one.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 19:09:38 by aaycan            #+#    #+#             */
/*   Updated: 2025/06/27 19:49:11 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_built_in_commands(t_command *cmd, t_gc *gc, char *line)
{
	if (!cmd || !cmd->argv || !cmd->argv[0])
		return;
	if (ft_strcmp(cmd->argv[0], "echo") == 0)
		echo_command(cmd);
	else if (ft_strcmp(cmd->argv[0], "pwd") == 0)
		pwd_command();
	else if (ft_strcmp(cmd->argv[0], "cd") == 0)
		cd_command(cmd);
	else if (ft_strcmp(cmd->argv[0], "exit") == 0)
		exit_command(cmd, gc, line);
}
