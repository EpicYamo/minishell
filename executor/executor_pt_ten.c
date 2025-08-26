/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_pt_ten.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 21:39:50 by aaycan            #+#    #+#             */
/*   Updated: 2025/08/26 21:56:38 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <unistd.h>
#include <readline/readline.h>
#include <stdlib.h>

void	init_non_built_in_data_struct(t_com_data_set *data_set, t_gc *gc,
	char **formatted_line, t_env **env_list)
{
	(*data_set).gc = gc;
	(*data_set).formatted_line = formatted_line;
	(*data_set).env_list = env_list;
}

void	execve_fail_handler(t_command *cmd, char *path,
	char **envp, t_com_data_set data_set)
{
	write(2, "Y-Shell: ", 9);
	write(2, cmd->argv[0], ft_strlen(cmd->argv[0]));
	write(2, ": command not found\n", 20);
	rl_clear_history();
	gc_collect_all(data_set.gc);
	free_string_array(data_set.formatted_line);
	free_env_list(*data_set.env_list);
	free(path);
	free_string_array(envp);
}
