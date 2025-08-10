/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 19:05:27 by aaycan            #+#    #+#             */
/*   Updated: 2025/08/10 21:13:03 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <readline/readline.h>

static void	shell_loop(t_env **env_list, int *exit_status);
static void	formatted_line_loop(char *line, t_env **env_list,
				char **formatted_line, int *exit_status);
static void	process_formatted_line(char **formatted_line, t_env **env_list,
				int *exit_status);

int	main(int argc, char **argv, char **envp)
{
	static int	exit_status = 0;
	t_env		*env_list;

	(void)argc;
	(void)argv;
	env_list = create_env_list(envp);
	print_banner();
	signal(SIGINT, handle_sigint_interactive);
	signal(SIGQUIT, SIG_IGN);
	shell_loop(&env_list, &exit_status);
	free_env_list(env_list);
	return (0);
}

static void	shell_loop(t_env **env_list, int *exit_status)
{
	char	*line;
	char	**formatted_line;

	while (1)
	{
		line = readline("\001\033[0;32m\002🜁 Y-Shell> \001\033[0m\002");
		if (!line)
		{
			write(1, "exit\n", 5);
			rl_clear_history();
			break ;
		}
		formatted_line = ft_split(line, '\n');
		free(line);
		if (!formatted_line)
		{
			write(2, "Allocation Error, Skipping the command...", 41);
			(*exit_status) = 2;
			continue ;
		}
		process_formatted_line(formatted_line, env_list, exit_status);
	}
}

static void	process_formatted_line(char **formatted_line, t_env **env_list,
	int *exit_status)
{
	int	i;

	i = -1;
	while (formatted_line[++i])
		formatted_line_loop(formatted_line[i], env_list, formatted_line,
			exit_status);
	free_string_array(formatted_line);
}

static void	formatted_line_loop(char *line, t_env **env_list,
	char **formatted_line, int *exit_status)
{
	t_io		shell_io;
	char		**tokens;
	t_command	*cmd;
	t_gc		*gc;
	t_interpret	interpret_set;

	if (init_garbage_collector_safe(&gc, line, exit_status, &shell_io) != 0)
		return ;
	if (init_interpret_set(gc, &interpret_set, line) != 0)
		return ;
	tokens = split_tokens(line, gc, (*env_list), interpret_set);
	if (!tokens)
		(*exit_status) = 2;
	else
		cmd = parse_tokens(tokens, gc, &shell_io, interpret_set);
	if (((*exit_status) != 2) && (!cmd) && ((*exit_status) != 130))
		(*exit_status = 2);
	else if ((*exit_status != 2) && (cmd != NULL))
	{
		cmd->io = &shell_io;
		(*exit_status) = command_executor(cmd, gc, formatted_line, env_list);
	}
	reinstate_shell(gc);
}
