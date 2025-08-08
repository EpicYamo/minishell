/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 03:25:16 by aaycan            #+#    #+#             */
/*   Updated: 2025/08/08 18:23:00 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <unistd.h>
#include <signal.h>
#include <readline/readline.h>
#include <stdlib.h>

void	handle_sigint_interactive(int signum)
{
	if (signum == SIGINT)
	{
		rl_replace_line("", 0);
		write(1, "\n", 1);
		rl_on_new_line();
		rl_redisplay();
	}
}

void	handle_sigint_heredoc(int signum)
{
	if (signum == SIGINT)
	{
		unlink("/tmp/tmpfileAXO135FJnT9AcnAJkKEALGKWKmZ.txt");
		write(1, "\n", 1);
		exit(128 + SIGINT);
	}
}
