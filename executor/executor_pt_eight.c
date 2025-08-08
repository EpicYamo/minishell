/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_pt_eight.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 15:37:17 by aaycan            #+#    #+#             */
/*   Updated: 2025/08/08 15:38:23 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdio.h>
#include <unistd.h>

int	skip_command(t_command **cmd)
{
	if (((*cmd)->argv == NULL) || ((*cmd)->argv[0] == NULL))
	{
		(*cmd) = (*cmd)->next;
		return (1);
	}
	return (0);
}

int	apply_pipe(t_io *io)
{
	if (pipe((*io).pipe_fd) == -1)
	{
		perror("pipe");
		return (1);
	}
	return (0);
}
