/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_pt_three.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 02:34:45 by aaycan            #+#    #+#             */
/*   Updated: 2025/08/26 22:36:27 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "fcntl.h"
#include <unistd.h>
#include <readline/readline.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>

static void	handle_heredoc_pt_two(char *delimiter, int fd,
				t_garbages all_garbage);
static void	write_heredoc_error(char *delimiter);
static int	fork_proc_read_input(t_command *cmd, char *delimiter,
				char *tempfile, t_garbages all_garbage);

int	handle_heredoc(t_command *cmd, char **tokens, size_t *i,
	t_garbages all_garbage)
{
	char	*delimiter;
	char	*tempfile;

	if ((cmd->infile != NULL) && (cmd->heredoc == 1))
		unlink(cmd->infile);
	delimiter = tokens[++(*i)];
	tempfile = ft_strdup("/tmp/tmpfileAXO135FJnT9AcnAJkKEALGKWKmZ.txt");
	if (!tempfile)
	{
		perror(tempfile);
		return (-1);
	}
	else
		gc_add(all_garbage.gc, tempfile);
	signal(SIGINT, SIG_IGN);
	if (fork_proc_read_input(cmd, delimiter, tempfile, all_garbage) != 1)
		return (-1);
	if (WIFEXITED(*cmd->io->exit_stat_ptr))
		(*cmd->io->exit_stat_ptr) = WEXITSTATUS((*cmd->io->exit_stat_ptr));
	if ((*cmd->io->exit_stat_ptr) == 130)
		return (-1);
	cmd->infile = tempfile;
	cmd->heredoc = 1;
	(*i)++;
	return (1);
}

static int	fork_proc_read_input(t_command *cmd, char *delimiter,
	char *tempfile, t_garbages all_garbage)
{
	int		fd;
	pid_t	proc_pid;

	fd = open(tempfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		write(2, "Error: creating heredoc file\n", 29);
		return (-1);
	}
	proc_pid = fork();
	if (proc_pid == 0)
		handle_heredoc_pt_two(delimiter, fd, all_garbage);
	close(fd);
	if (proc_pid < 0)
	{
		perror("fork");
		return (-1);
	}
	if (waitpid(proc_pid, cmd->io->exit_stat_ptr, 0) == -1)
	{
		perror("waitpid");
		return (-1);
	}
	return (1);
}

static void	handle_heredoc_pt_two(char *delimiter, int fd,
	t_garbages all_garbage)
{
	char	*line;

	signal(SIGINT, handle_sigint_heredoc);
	line = readline("> ");
	while (line != NULL)
	{
		if ((ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0)
			&& (line[ft_strlen(delimiter)] == '\0'))
		{
			free(line);
			break ;
		}
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
		line = readline("> ");
	}
	if (!line)
		write_heredoc_error(delimiter);
	free_string_array(all_garbage.formatted_line);
	gc_collect_all(all_garbage.gc);
	free_env_list(all_garbage.env_list);
	close(fd);
	exit(EXIT_SUCCESS);
}

static void	write_heredoc_error(char *delimiter)
{
	write(2, "Y-Shell: warning: here-document", 31);
	write(2, " delimited by end-of-file (wanted \'", 35);
	write(2, delimiter, ft_strlen(delimiter));
	write(2, "\')\n", 3);
}
