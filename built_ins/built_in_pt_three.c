/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_pt_three.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 13:09:45 by aaycan            #+#    #+#             */
/*   Updated: 2025/08/09 17:28:44 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <readline/readline.h>

static void	exit_command_pt_two(t_command *cmd, int *arg_err_flag,
				int *exit_code);
static void	execute_env_outfile(t_command *cmd, t_env *env);
static int	check_exit_code_validity(char *argv);

void	exit_command(t_command *cmd, t_gc *gc, char **formatted_line,
				t_env *env_list)
{
	int	exit_code;
	int	arg_err_flag;

	exit_code = 0;
	arg_err_flag = 0;
	printf("exit\n");
	exit_command_pt_two(cmd, &arg_err_flag, &exit_code);
	rl_clear_history();
	gc_collect_all(gc);
	free_string_array(formatted_line);
	free_env_list(env_list);
	exit(exit_code);
}

static void	exit_command_pt_two(t_command *cmd, int *arg_err_flag,
				int *exit_code)
{
	if (cmd->argv[1])
	{
		(*exit_code) = ft_atoi(cmd->argv[1]);
		if (check_exit_code_validity(cmd->argv[1]) != 0)
		{
			write(2, "Y-Shell: exit: ", 15);
			write(2, cmd->argv[1], ft_strlen(cmd->argv[1]));
			write(2, ": numeric argument required\n", 28);
			(*arg_err_flag) = 1;
			(*exit_code) = 2;
		}
	}
	if (((*arg_err_flag) == 0) && (cmd->argv[1] != NULL)
		&& (cmd->argv[2] != NULL))
	{
		write(2, "Y-Shell: exit: too many arguments\n", 34);
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

static int	check_exit_code_validity(char *argv)
{
	int	i;

	i = -1;
	if (argv[0] == '+' || argv[0] == '-')
		i++;
	while (argv[++i])
	{
		if (ft_isdigit(argv[i]) == 0)
			return (1);
	}
	return (0);
}
