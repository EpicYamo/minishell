/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_pt_two.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 18:04:26 by aaycan            #+#    #+#             */
/*   Updated: 2025/08/26 19:14:26 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <signal.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <readline/readline.h>

static void	io_prep(t_command *cmd);
static void	close_fds_for_child_proc(t_command *cmd);

void	exec_built_in_com_in_child_proc(t_command *cmd, t_gc *gc,
	char **formatted_line, t_env **env_list)
{
	pid_t	proc_pid;
	int		exit_status;

	io_prep(cmd);
	proc_pid = fork();
	if (proc_pid == 0)
	{
		if (cmd->next)
			dup2(cmd->io->pipe_fd[1], STDOUT_FILENO);
		exit_status = (*cmd->io->exit_stat_ptr);
		//if (ft_strcmp(cmd->argv[0], "exit") == 0)
		//	execute_exit_command_in_child_proc();  (execute exit seperately)
		execute_built_in_commands(cmd, gc, formatted_line, env_list);
		close_fds_for_child_proc(cmd);
		rl_clear_history();
		gc_collect_all(gc);
		free_string_array(formatted_line);
		free_env_list(*env_list);
		exit(exit_status);
	}
	if (proc_pid > 0)
		cmd->io->pids[(cmd->io->proc_count)++] = proc_pid;
	close(cmd->io->pipe_fd[1]);
	if (proc_pid < 0)
		perror("fork");
}

static void	io_prep(t_command *cmd)
{
	if (cmd->io->prev_fd != -1)
	{
		close(cmd->io->prev_fd);
		cmd->io->prev_fd = -1;
		dup2(cmd->io->original_stdout, STDOUT_FILENO);
	}
	if (cmd->next)
		cmd->io->prev_fd = cmd->io->pipe_fd[0];
	else
		close(cmd->io->pipe_fd[0]);
}

static void	close_fds_for_child_proc(t_command *cmd)
{
	close(cmd->io->original_stdin);
	close(cmd->io->original_stdout);
	close(cmd->io->pipe_fd[1]);
	if (cmd->next)
		close(cmd->io->pipe_fd[0]);
}

int	execute_single_built_in_command(t_command **cmd, t_gc *gc,
	char **formatted_line, t_env **env_list)
{
	if (skip_command(cmd) != 0)
		return (0);
	if ((*cmd) && ((*cmd)->next == NULL) && (is_builtin((*cmd)->argv[0])))
	{
		if (apply_exit_status_token((*cmd), gc) != 0)
			return (1);
		execute_built_in_commands((*cmd), gc, formatted_line, env_list);
		(*cmd) = (*cmd)->next;
	}
	return (0);
}
