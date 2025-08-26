/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_pt_three.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 17:21:42 by aaycan            #+#    #+#             */
/*   Updated: 2025/08/26 21:59:56 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

static void	exec_non_built_in_com_in_child_proc(t_command *cmd, char *path,
				char **envp, t_com_data_set data_set);
static void	replace_process_with_execution(t_command *cmd, char *path,
				char **envp, t_com_data_set data_set);
static int	create_path_and_envp(t_command *cmd, t_env *env_list,
				char **path, char ***envp);
static void	close_fds(t_command *cmd);

void	execute_non_built_in_command(t_command *cmd, t_gc *gc,
	char **formatted_line, t_env **env_list)
{
	pid_t			proc_pid;
	char			**envp;
	char			*path;
	t_com_data_set	data_set;

	if (create_path_and_envp(cmd, (*env_list), &path, &envp) != 0)
	{
		close_fds_error_condition(cmd);
		return ;
	}
	init_non_built_in_data_struct(&data_set, gc, formatted_line, env_list);
	proc_pid = fork();
	if (proc_pid == 0)
		exec_non_built_in_com_in_child_proc(cmd, path, envp, data_set);
	if (proc_pid < 0)
	{
		perror("fork");
		(*cmd->io->exit_stat_ptr) = 1;
		return ;
	}
	cmd->io->pids[cmd->io->proc_count] = proc_pid;
	cmd->io->proc_count += 1;
	free(path);
	free_string_array(envp);
	close_fds(cmd);
}

static void	exec_non_built_in_com_in_child_proc(t_command *cmd, char *path,
	char **envp, t_com_data_set data_set)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (cmd->next)
		dup2(cmd->io->pipe_fd[1], STDOUT_FILENO);
	close(cmd->io->pipe_fd[1]);
	close(cmd->io->pipe_fd[0]);
	if (cmd->io->prev_fd != -1)
	{
		dup2(cmd->io->prev_fd, STDIN_FILENO);
		close(cmd->io->prev_fd);
		cmd->io->prev_fd = -1;
	}
	setup_infile_redirect(cmd, (*data_set.env_list));
	setup_outfile_redirect(cmd);
	replace_process_with_execution(cmd, path, envp, data_set);
}

static void	replace_process_with_execution(t_command *cmd, char *path,
	char **envp, t_com_data_set data_set)
{
	if (execve(path, cmd->argv, envp) == -1)
	{
		execve_fail_handler(cmd, path, envp, data_set);
		exit(127);
	}
}

static void	close_fds(t_command *cmd)
{
	if (cmd->io->prev_fd != -1)
	{
		close(cmd->io->prev_fd);
		cmd->io->prev_fd = -1;
	}
	if (cmd->next)
		cmd->io->prev_fd = cmd->io->pipe_fd[0];
	else
		close(cmd->io->pipe_fd[0]);
	close(cmd->io->pipe_fd[1]);
}

static int	create_path_and_envp(t_command *cmd, t_env *env_list,
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
