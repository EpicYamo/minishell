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
#include <string.h>  // strcmp
#include <unistd.h>  // write, chdir, getcwd
#include <stdio.h>   // printf, perror
#include <stdlib.h>  // exit
#include <errno.h>

extern char **environ;

static void execute_built_in_commands(t_command *cmd);
static int is_builtin(const char *cmd);

int	command_executor(t_command *cmd)
{
	if (!cmd || !cmd->argv || !cmd->argv[0])
		return (1);
	if (is_builtin(cmd->argv[0]))
	{
		execute_built_in_commands(cmd);
		return (0);
	}
	// execve processos will be made right here...
	return (1);
}

static int is_builtin(const char *cmd)
{
	int	flag;

	flag = strcmp(cmd, "echo");
	flag = strcmp(cmd, "cd");
	flag = strcmp(cmd, "pwd");
	flag = strcmp(cmd, "export");
	flag = strcmp(cmd, "unset");
	flag = strcmp(cmd, "env");
	flag = strcmp(cmd, "exit");
	return(flag);
	// if flag is init as '1' return process will cout as the parameter of the function if not it wont so use
	// variadic variables for reallocating the memory.
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
	else if (strcmp(cmd->argv[0], "env") == 0)
	{
		for (char **env = environ; *env; ++env)
			printf("%s\n", *env);
	}
	else if (strcmp(cmd->argv[0], "export") == 0)
	{
		if (!cmd->argv[1])
		{
			for (char **env = environ; *env; ++env)
				printf("declare -x %s\n", *env);
		}
		else
		{
			// Support for the basic key value.
			putenv(cmd->argv[1]);
		}
	}
	else if (strcmp(cmd->argv[0], "unset") == 0)
	{
		if (cmd->argv[1])
			env_unset(cmd->argv[1]);
	}
	else if (strcmp(cmd->argv[0], "exit") == 0)
	{
		int exit_code = 0;
		if (cmd->argv[1])
			exit_code = atoi(cmd->argv[1]);
		exit(exit_code);
	}
}

void	env_unset(const char *var)
{
	extern char **environ;
	int i = 0;
	int len = strlen(var);
	while (environ[i])
	{
		if (strncmp(environ[i], var, len) == 0 && environ[i][len] == '=')
		{
			while (environ[i])
			{
				environ[i] = environ[i + 1];
				i++;
			}
			break;
		}
		i++;
	}
}
