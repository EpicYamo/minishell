/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_pt_six.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 01:47:49 by aaycan            #+#    #+#             */
/*   Updated: 2025/08/01 03:20:02 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

static int	calculate_infile_len(t_command *cmd);
static int	save_infile(t_command *cmd, char *infile);
static int	save_infile_pt_two(int fd, char *infile);
static int	search_infile(char *infile);

int	expand_dollar_sign_in_heredoc(t_command *cmd, t_env *env_list)
{
	int		infile_len;
	char	*infile;

	infile_len = calculate_infile_len(cmd);
	if (infile_len == -1)
		return (1);
	if (infile_len == 0)
		return (0);
	infile = malloc(sizeof(char) * (infile_len + 1));
	if (!infile)
		return (1);
	if (save_infile(cmd, infile) != 0)
		return (1);
	if (search_infile(infile) == 0)
		return (0);
	if (modify_apply_heredoc_file(cmd, infile, env_list) != 0)
		return (1);
	return (0);
}

static int	calculate_infile_len(t_command *cmd)
{
	char	buffer[42];
	long	bytes_read;
	size_t	infile_len;
	int		fd;

	fd = open(cmd->infile, O_RDONLY);
	if (fd == -1)
		return (-1);
	bytes_read = 42;
	infile_len = 0;
	while (bytes_read != 0)
	{
		bytes_read = read(fd, buffer, 42);
		if (bytes_read == -1)
		{
			close(fd);
			return (-1);
		}
		infile_len += bytes_read;
	}
	close(fd);
	return (infile_len);
}

static int	save_infile(t_command *cmd, char *infile)
{
	int	fd;

	fd = open(cmd->infile, O_RDONLY);
	if (fd == -1)
	{
		free(infile);
		return (1);
	}
	if (save_infile_pt_two(fd, infile) != 0)
	{
		close(fd);
		return (1);
	}
	close(fd);
	return (0);
}

static int	save_infile_pt_two(int fd, char *infile)
{
	char	buffer[42];
	long	bytes_read;
	int		i;
	int		j;

	bytes_read = 42;
	i = 0;
	while (bytes_read != 0)
	{
		j = -1;
		bytes_read = read(fd, buffer, 41);
		if (bytes_read == -1)
		{
			free(infile);
			return (1);
		}
		buffer[bytes_read] = '\0';
		while (buffer[++j])
		{
			infile[i] = buffer[j];
			i++;
		}
	}
	infile[i] = '\0';
	return (0);
}

static int	search_infile(char *infile)
{
	int	i;

	i = 0;
	while (infile[i])
	{
		if (infile[i] == '$')
			return (1);
		i++;
	}
	free(infile);
	return (0);
}
