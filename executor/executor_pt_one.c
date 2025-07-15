/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_pt_one.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 03:26:26 by aaycan            #+#    #+#             */
/*   Updated: 2025/07/15 16:32:50 by aaycan           ###   ########.fr       */
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

static void	setup_redirects(t_command *cmd);
static int	env_list_size(t_env *env);
static char	**get_envp(t_env *env_list);
static void	ft_free_split(char **split);
char		*get_env_value(t_env *env_list, const char *key);
char		*resolve_path(const char *cmd, t_env *env_list);

void	command_executor(t_command *cmd, t_gc *gc, char **formatted_line,
				t_env *env_list)
{
	int		pipefd[2];
	int		prev_fd = -1;
	pid_t	pid;

	while (cmd)
	{
		if (!cmd->argv || !cmd->argv[0])
		{
			cmd = cmd->next;
			continue ;
		}
		if (cmd->next && (pipe(pipefd) == -1))
		{
			perror("pipe");
			return ;
		}
		pid = fork();
		if (pid == 0)
		{
			if (prev_fd != -1)
			{
				dup2(prev_fd, STDIN_FILENO);
				close(prev_fd);
			}
			if (cmd->next)
			{
				close(pipefd[0]);
				dup2(pipefd[1], STDOUT_FILENO);
				close(pipefd[1]);
			}
			setup_redirects(cmd);
			if (is_builtin(cmd->argv[0]))
				execute_built_in_commands(cmd, gc, formatted_line, env_list);
			else
			{
				char *path = resolve_path(cmd->argv[0], env_list);
				if (execve(path, cmd->argv, get_envp(env_list)) == -1)
				{
					perror("execve");
					exit(127);
				}
			}
		}
		else if (pid < 0)
		{
			perror("fork");
			return ;
		}
		if (prev_fd != -1)
			close(prev_fd);
		if (cmd->next)
		{
			close(pipefd[1]);
			prev_fd = pipefd[0];
		}
		if (cmd->heredoc == 1)
			unlink(cmd->infile);
		cmd = cmd->next;
	}
	while (wait(NULL) > 0)
		;
}

static void	setup_redirects(t_command *cmd)
{
	int	fd;

	if (cmd->infile && cmd->heredoc == 0)
	{
		fd = open(cmd->infile, O_RDONLY);
		if (fd == -1)
			perror("infile");
		else
		{
			dup2(fd, STDIN_FILENO);
			close(fd);
		}
	}
	if (cmd->outfile)
	{
		if (cmd->append == 1)
			fd = open(cmd->outfile, O_WRONLY | O_APPEND, 0644);
		else
			fd = open(cmd->outfile, O_WRONLY | O_TRUNC, 0644);
		if (fd == -1)
			perror("outfile");
		else
		{
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
	}
}

static int	env_list_size(t_env *env)
{
	int	count = 0;

	while (env)
	{
		if (env->key && env->value)
			count++;
		env = env->next;
	}
	return (count);
}

static char	**get_envp(t_env *env_list)
{
	int		i = 0;
	int		size = env_list_size(env_list);
	char	**envp = malloc(sizeof(char *) * (size + 1));
	char	*tmp;

	if (!envp)
		return (NULL);
	while (env_list)
	{
		if (env_list->key && env_list->value)
		{
			tmp = ft_strjoin(env_list->key, "=");
			envp[i] = ft_strjoin(tmp, env_list->value);
			free(tmp);
			i++;
		}
		env_list = env_list->next;
	}
	envp[i] = NULL;
	return (envp);
}

static void	ft_free_split(char **split)
{
	int	i = 0;

	if (!split)
		return ;
	while (split[i])
		free(split[i++]);
	free(split);
}

char	*get_env_value(t_env *env_list, const char *key)
{
	while (env_list)
	{
		if (env_list->key && strcmp(env_list->key, key) == 0)
			return (env_list->value);
		env_list = env_list->next;
	}
	return (NULL);
}

char	*resolve_path(const char *cmd, t_env *env_list)
{
	char	**paths;
	char	*full_path;
	char	*tmp;
	int		i;

	if (access(cmd, X_OK) == 0)
		return (ft_strdup(cmd));
	char *path_value = get_env_value(env_list, "PATH");
	if (!path_value)
		return (NULL);
	paths = ft_split(path_value, ':');
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		tmp = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin(tmp, cmd);
		free(tmp);
		if (access(full_path, X_OK) == 0)
		{
			ft_free_split(paths);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	ft_free_split(paths);
	return (NULL);
}
