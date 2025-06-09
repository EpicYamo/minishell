/* ************************************************************************************** */
/*                                                                                        */
/*                                                                   :::      ::::::::    */
/*   minishell.c                                                   :+:      :+:    :+:    */
/*                                                               +:+ +:+         +:+      */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr>              +#+  +:+       +#+         */
/*                                                           +#+#+#+#+#+   +#+            */
/*   Created: 2025/05/25 18:08:31 by aaycan                       #+#    #+#              */
/*   Updated: 2025/05/25 18:08:31 by aaycan                      ###   ########.tr        */
/*                                                                                        */
/* ************************************************************************************** */

#include "minishell.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>

static void	print_commands(t_command *cmd);

int	main(void)
{
	char		*line;
	char		**tokens;
	t_command	*cmd;

	print_banner();
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
	while (1)
    {
        line = readline("\001\033[0;32m\002🜁 Y-Shell> \001\033[0m\002");
        tokens = NULL;
		cmd = NULL;
		if (!line)
		{
			printf("exit\n");
			rl_clear_history();
			break;
		}
        if (line)
			add_history(line);
		tokens = split_tokens(line);
		if (!tokens)
		{
			printf("\033[0;31mSystem error: memory allocation failed in \"split_tokens\" function\033[0m\n");
			free(line);
			continue;
		}
		if (tokens)
		{
			free(line);
			cmd = parse_tokens(tokens);
		}
		if (!cmd)
		{
			printf("\033[0;31mSystem error: memory allocation failed in \"parse_tokens\" function\033[0m\n");
			free(line);
			//free tokens in a loop
			free(tokens);
			continue;
		}
		if (cmd)
		{
			print_commands(cmd);
			command_executor(cmd);
			free_commands(cmd);
			free(tokens);
		}
		
    }
	return (0);
}

static void	print_commands(t_command *cmd)
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
