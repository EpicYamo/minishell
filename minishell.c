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
static void free_commands(t_command *cmd);

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
        if (!line)
		{
			printf("exit\n");
			break;
		}
        if (line)
			add_history(line);
		tokens = split_tokens(line);
		if (tokens)
			cmd = parse_tokens(tokens);
		if (cmd)
		{
			print_commands(cmd);
			free_commands(cmd);
		}
		free(tokens);
        free(line);
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
		while (cmd->argv && cmd->argv[i])
		{
			printf("argv[%d]: \"%s\"\n", i, cmd->argv[i]);
			i++;
		}
		printf("infile   : %s\n", cmd->infile ? cmd->infile : "(none)");
		printf("outfile  : %s\n", cmd->outfile ? cmd->outfile : "(none)");
		printf("append   : %d\n", cmd->append);
		printf("heredoc  : %d\n", cmd->heredoc);
		cmd = cmd->next;
	}
}

static void free_commands(t_command *cmd)
{
	while (cmd)
	{
		t_command *next = cmd->next;
		size_t i = 0;
		if (cmd->argv)
		{
			while (cmd->argv[i])
				free(cmd->argv[i++]);			
			free(cmd->argv);
		}
		if (cmd->infile)
			free(cmd->infile);
		if (cmd->outfile)
			free(cmd->outfile);
		free(cmd);
		cmd = next;
	}
}
