/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_pt_eight.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 17:15:20 by aaycan            #+#    #+#             */
/*   Updated: 2025/07/22 17:26:26 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

static int	print_export_list_to_outfile_pt_two(t_command *cmd);

void	print_export_list_to_stdout(t_env *tmp)
{
	if (!tmp)
		return ;
	while (tmp)
	{
		if (tmp->key)
		{
			printf("declare -x %s", tmp->key);
			if (tmp->value)
				printf("=\"%s\"", tmp->value);
			printf("\n");
		}
		tmp = tmp->next;
	}
}

void	print_export_list_to_outfile(t_command *cmd, t_env *tmp)
{
	int	fd;

	fd = print_export_list_to_outfile_pt_two(cmd);
	if (fd == -1)
	{
		perror("Error opening file");
		return ;
	}
	while (tmp)
	{
		if (tmp->key)
		{
			write(fd, "declare -x ", 11);
			write(fd, tmp->key, ft_strlen(tmp->key));
			if (tmp->value)
			{
				write(fd, "=\"", 2);
				write(fd, tmp->value, ft_strlen(tmp->value));
				write(fd, "\"", 1);
			}
			write(fd, "\n", 1);
		}
		tmp = tmp->next;
	}
	close(fd);
}

static int	print_export_list_to_outfile_pt_two(t_command *cmd)
{
	int	fd;

	if (cmd->append == 0)
		fd = open(cmd->outfile, O_WRONLY, 0644);
	else
		fd = open(cmd->outfile, O_WRONLY | O_APPEND, 0644);
	return (fd);
}
