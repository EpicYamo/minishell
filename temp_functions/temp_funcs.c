/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   temp_funcs.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 19:07:31 by aaycan            #+#    #+#             */
/*   Updated: 2025/07/12 22:16:12 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdio.h>

void	print_commands(t_command *cmd)
{
	int	i;
	int	cmd_id;

	cmd_id = 1;
	while (cmd)
	{
		printf("\n\033[1;34m[ Command %d ]\033[0m\n", cmd_id++);
		i = 0;
		if (cmd->argv)
		{
			while (cmd->argv && cmd->argv[i])
			{
				printf("argv[%d]: \"%s\"\n", i, cmd->argv[i]);
				i++;
			}
		}
		printf("infile   : %s\n", cmd->infile);
		printf("outfile  : %s\n", cmd->outfile);
		printf("append   : %d\n", cmd->append);
		printf("heredoc  : %d\n", cmd->heredoc);
		cmd = cmd->next;
	}
}
