/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 03:25:44 by aaycan            #+#    #+#             */
/*   Updated: 2025/06/28 13:20:43 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>

static void	shell_loop(void);
static void	shell_loop_pt_two(char *line);

int	main(void)
{
	print_banner();
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
	shell_loop();
	return (0);
}

static void	shell_loop(void)
{
	char	*line;

	while (1)
    {
        line = readline("\001\033[0;32m\002🜁 Y-Shell> \001\033[0m\002");
		if (line == NULL)
		{
			printf("exit\n");
			rl_clear_history();
			break;
		}
        else if (line != NULL)
			add_history(line);
		shell_loop_pt_two(line);
		free(line);
    }
}

static void	shell_loop_pt_two(char *line)
{
	char		**tokens;
	t_command	*cmd;
	t_gc		*garbage_c;

	tokens = NULL;
	garbage_c = init_garbage_collector();
	if (garbage_c != NULL)
		tokens = split_tokens(line, garbage_c);
	if (tokens != NULL)
	{
		cmd = parse_tokens(tokens, garbage_c);
		if (cmd != NULL)
		{
			//print_commands(cmd);
			command_executor(cmd, garbage_c, line);
		}
	}
	gc_collect_all(garbage_c);
}
