/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_pt_one.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 03:26:26 by aaycan            #+#    #+#             */
/*   Updated: 2025/07/22 16:32:36 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/wait.h>
#include <fcntl.h>


void	command_executor(t_command *cmd, t_gc *gc, char **formatted_line,
				t_env *env_list)
{
	if (is_builtin(cmd->argv[0]))
		execute_built_in_commands(cmd, gc, formatted_line, env_list);
	// Message to chatgpt: this is how I handle the built-ins but when i use echo or export I directly write them from the stdout with write. so the outputs of those built ins might be faulty for the implementation of the code... I will change it if that is the case.
}