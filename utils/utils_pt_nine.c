/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_pt_nine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 18:16:49 by aaycan            #+#    #+#             */
/*   Updated: 2025/08/14 01:58:11 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <signal.h>
#include <unistd.h>

void	reinstate_shell(t_gc *gc)
{
	signal(SIGINT, handle_sigint_interactive);
	gc_collect_all(gc);
}

void	write_error_with_arg(t_command *cmd)
{
	write(2, "Y-Shell: ", 9);
	write(2, cmd->argv[0], ft_strlen(cmd->argv[0]));
}

int	if_executeable(t_command *cmd)
{
	int	i;

	i = 0;
	while (cmd->argv[0][i])
	{
		if (ft_isspace(cmd->argv[0][i]) != 1)
			break ;
		i++;
	}
	if (cmd->argv[0][i] == '.')
	{
		i++;
		if (cmd->argv[0][i] == '/')
		{
			if (access(cmd->argv[0], X_OK) == 0)
				return (1);
		}
	}
	return (0);
}
