/* ************************************************************************************** */
/*                                                                                        */
/*                                                                   :::      ::::::::    */
/*   z_temp_funcs.c                                                :+:      :+:    :+:    */
/*                                                               +:+ +:+         +:+      */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr>              +#+  +:+       +#+         */
/*                                                           +#+#+#+#+#+   +#+            */
/*   Created: 2025/06/16 00:10:39 by aaycan                       #+#    #+#              */
/*   Updated: 2025/06/16 00:10:39 by aaycan                      ###   ########.tr        */
/*                                                                                        */
/* ************************************************************************************** */

#include "minishell.h"
#include <stdio.h>

void	print_commands(t_command *cmd)
{
	int	i;
	int	cmd_id = 1;

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
		printf("infile   : %s\n", cmd->infile ? cmd->infile : "(none)");
		printf("outfile  : %s\n", cmd->outfile ? cmd->outfile : "(none)");
		printf("append   : %d\n", cmd->append);
		printf("heredoc  : %d\n", cmd->heredoc);
		cmd = cmd->next;
	}
}