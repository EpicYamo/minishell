/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_pt_ten.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 21:39:50 by aaycan            #+#    #+#             */
/*   Updated: 2025/08/26 23:43:44 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <unistd.h>
#include <readline/readline.h>
#include <stdlib.h>
#include <stdio.h>

void	init_non_built_in_data_struct(t_com_data_set *data_set, t_gc *gc,
	char **formatted_line, t_env **env_list)
{
	(*data_set).gc = gc;
	(*data_set).formatted_line = formatted_line;
	(*data_set).env_list = env_list;
}

void	execve_fail_handler(char *path, char **envp, t_com_data_set data_set)
{
	rl_clear_history();
	gc_collect_all(data_set.gc);
	free_string_array(data_set.formatted_line);
	free_env_list(*data_set.env_list);
	free(path);
	free_string_array(envp);
}

int	create_path_and_envp(t_command *cmd, t_env *env_list,
	char **path, char ***envp)
{
	if (test_path(cmd) != 0)
		return (1);
	(*path) = resolve_path(cmd->argv[0], env_list);
	if (!(*path))
	{
		write_error_with_arg(cmd);
		write(2, ": command not found\n", 20);
		(*cmd->io->exit_stat_ptr) = 127;
		return (1);
	}
	(*envp) = get_envp(env_list);
	if (!(*envp))
	{
		free((*path));
		perror("envp");
		(*cmd->io->exit_stat_ptr) = 127;
		return (1);
	}
	return (0);
}
