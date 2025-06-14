/* ************************************************************************************** */
/*                                                                                        */
/*                                                                   :::      ::::::::    */
/*   executor_pt_one.c                                             :+:      :+:    :+:    */
/*                                                               +:+ +:+         +:+      */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr>              +#+  +:+       +#+         */
/*                                                           +#+#+#+#+#+   +#+            */
/*   Created: 2025/06/08 21:35:13 by aaycan                       #+#    #+#              */
/*   Updated: 2025/06/08 21:35:13 by aaycan                      ###   ########.tr        */
/*                                                                                        */
/* ************************************************************************************** */

#include "minishell.h"
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

static void	execute_built_in_commands(t_command *cmd);
static int	is_builtin(const char *cmd);

void	command_executor(t_command *cmd)
{
	if (!cmd || !cmd->argv || !cmd->argv[0])
		return;
	if (is_builtin(cmd->argv[0]))
		execute_built_in_commands(cmd);
	// execve processos will be made right here...
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

static void	execute_built_in_commands(t_command *cmd)
{
	if (!cmd || !cmd->argv || !cmd->argv[0])
		return;
	if (strcmp(cmd->argv[0], "echo") == 0)
	{
		int i = 1;
		int newline = 1;
		if (cmd->argv[1] && strcmp(cmd->argv[1], "-n") == 0)
		{
			newline = 0;
			i++;
		}
		while (cmd->argv[i])
		{
			printf("%s", cmd->argv[i]);
			if (cmd->argv[i + 1])
				printf(" ");
			i++;
		}
		if (newline)
			printf("\n");
	}
	else if (strcmp(cmd->argv[0], "pwd") == 0)
	{
		char	cwd[1024];
		if (getcwd(cwd, sizeof(cwd)))
			printf("%s\n", cwd);
	}
	else if (strcmp(cmd->argv[0], "cd") == 0)
	{
		if (cmd->argv[1])
		{
			if (chdir(cmd->argv[1]) != 0)
				perror("cd");
		}
		else
		{
			char	*home;

			home = getenv("HOME");
			if (!home)
				printf("cd: HOME not set\n");
			if (chdir(home) != 0)
				perror("cd");
		}
	}
	else if (strcmp(cmd->argv[0], "exit") == 0)
	{
		int exit_code = 0;
		if (cmd->argv[1])
			exit_code = atoi(cmd->argv[1]);
		exit(0);
	}
}
