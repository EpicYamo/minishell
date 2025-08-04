/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_pt_one.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 19:09:38 by aaycan            #+#    #+#             */
/*   Updated: 2025/08/04 18:18:14 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdio.h>

void	execute_built_in_commands(t_command *cmd, t_gc *gc,
	char **formatted_line, t_env *env_list)
{
	if (!cmd || !cmd->argv || !cmd->argv[0])
		return ;
	(*cmd->io->exit_stat_ptr) = 0;
	if (ft_strcmp(cmd->argv[0], "echo") == 0)
		echo_command(cmd);
	else if (ft_strcmp(cmd->argv[0], "pwd") == 0)
		pwd_command(cmd);
	else if (ft_strcmp(cmd->argv[0], "cd") == 0)
		cd_command(cmd, env_list);
	else if (ft_strcmp(cmd->argv[0], "exit") == 0)
		exit_command(cmd, gc, formatted_line, env_list);
	else if (ft_strcmp(cmd->argv[0], "env") == 0)
		env_command(cmd, env_list);
	else if (ft_strcmp(cmd->argv[0], "export") == 0)
		export_command(cmd, &env_list);
	else if (ft_strcmp(cmd->argv[0], "unset") == 0)
		unset_command(cmd, &env_list);
}

int	is_builtin(const char *cmd)
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
