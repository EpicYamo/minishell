/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_pt_three.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 17:21:42 by aaycan            #+#    #+#             */
/*   Updated: 2025/08/13 15:33:25 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

static void	exec_non_built_in_com_in_child_proc(t_command *cmd, char *path,
				char **envp, t_env *env_list);
static void	replace_process_with_execution(t_command *cmd, char *path,
				char **envp);
static int	create_path_and_envp(t_command *cmd, t_env *env_list,
				char **path, char ***envp);
static void	close_fds(t_command *cmd);

void	execute_non_built_in_command(t_command *cmd, t_env *env_list)
{
	pid_t	proc_pid;
	char	**envp;
	char	*path;

	if (create_path_and_envp(cmd, env_list, &path, &envp) != 0)
		return ;
	proc_pid = fork();
	if (proc_pid == 0)
		exec_non_built_in_com_in_child_proc(cmd, path, envp, env_list);
	if (proc_pid < 0)
	{
		perror("fork");
		return ;
	}
	cmd->io->pids[cmd->io->proc_count] = proc_pid;
	cmd->io->proc_count += 1;
	free(path);
	free_string_array(envp);
	close_fds(cmd);
}

static void	exec_non_built_in_com_in_child_proc(t_command *cmd, char *path,
	char **envp, t_env *env_list)
{
	signal(SIGINT, handle_sigint_interactive);
	signal(SIGQUIT, SIG_DFL);
	if (cmd->next)
		dup2(cmd->io->pipe_fd[1], STDOUT_FILENO);
	close(cmd->io->pipe_fd[1]);
	close(cmd->io->pipe_fd[0]);
	if (cmd->io->prev_fd != -1)
	{
		dup2(cmd->io->prev_fd, STDIN_FILENO);
		close(cmd->io->prev_fd);
	}
	setup_infile_redirect(cmd, env_list);
	setup_outfile_redirect(cmd);
	replace_process_with_execution(cmd, path, envp);
}

static void	replace_process_with_execution(t_command *cmd, char *path,
	char **envp)
{
	if (execve(path, cmd->argv, envp) == -1)
	{
		write(2, "Y-Shell: ", 9);
		write(2, cmd->argv[0], ft_strlen(cmd->argv[0]));
		write(2, ": command not found\n", 20);
		exit(127);
	}
}

static void	close_fds(t_command *cmd)
{
	if (cmd->io->prev_fd != -1)
		close(cmd->io->prev_fd);
	if (cmd->next != NULL)
		cmd->io->prev_fd = cmd->io->pipe_fd[0];
	else
		close(cmd->io->pipe_fd[0]);
	close(cmd->io->pipe_fd[1]);
}

static int	create_path_and_envp(t_command *cmd, t_env *env_list,
	char **path, char ***envp)
{
	(*path) = resolve_path(cmd->argv[0], env_list);
	if (!(*path))
	{
		write(2, "Y-Shell: ", 9);
		write(2, cmd->argv[0], ft_strlen(cmd->argv[0]));
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
