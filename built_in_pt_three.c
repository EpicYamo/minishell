/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_pt_three.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 13:09:45 by aaycan            #+#    #+#             */
/*   Updated: 2025/06/28 22:43:54 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <readline/readline.h>

static void	exit_command_pt_two(t_command *cmd, int *arg_err_flag, int *exit_code);
static void	execute_env_outfile(t_command *cmd, t_env *env);

void	exit_command(t_command *cmd, t_gc *gc, char *line, t_env *env_list)
{
	int	exit_code;
	int	arg_err_flag;

	exit_code = 0;
	arg_err_flag = 0;
	printf("exit\n");
	exit_command_pt_two(cmd, &arg_err_flag, &exit_code);
	rl_clear_history();
	gc_collect_all(gc);
	free(line);
	free_env_list(env_list);
	exit(exit_code);
}

static void	exit_command_pt_two(t_command *cmd, int *arg_err_flag, int *exit_code)
{
	int	i;

	if (cmd->argv[1])
	{
		(*exit_code) = ft_atoi(cmd->argv[1]);
		i = -1;
		while (cmd->argv[1][++i])
		{
			if (ft_isdigit(cmd->argv[1][i]) == 0)
			{
				printf("Y-Shell: exit: %s: numeric argument required\n", cmd->argv[1]);
				(*arg_err_flag) = 1;
				(*exit_code) = 2;
				break;
			}
		}
	}
	if (((*arg_err_flag) == 0) && (cmd->argv[1] != NULL)
		&& (cmd->argv[2] != NULL))
	{
		printf("Y-Shell: exit: too many arguments\n");
		(*exit_code) = 1;
	}
}

void	env_command(t_command *cmd, t_env *env)
{
	if (cmd->outfile)
		execute_env_outfile(cmd, env);
	else
	{
		while (env)
		{
			if (env->key && env->value)
				printf("%s=%s\n", env->key, env->value);
			env = env->next;
		}
	}
	
}

static void	execute_env_outfile(t_command *cmd, t_env *env)
{
	int	fd;

	if (cmd->append == 0)
		fd = open(cmd->outfile, O_WRONLY, 0644);
	else
		fd = open(cmd->outfile, O_WRONLY | O_APPEND, 0644);
	if (fd == -1)
	{
        perror("Error opening file");
        return ;
	}
	while (env)
	{
		if (env->key && env->value)
		{
			write(fd, env->key, ft_strlen(env->key));
			write(fd, "=", 1);
			write(fd, env->value, ft_strlen(env->value));
			write(fd, "\n", 1);
		}
		env = env->next;
	}
	close(fd);
}
