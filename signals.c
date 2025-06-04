/* ************************************************************************************** */
/*                                                                                        */
/*                                                                   :::      ::::::::    */
/*   signals.c                                                     :+:      :+:    :+:    */
/*                                                               +:+ +:+         +:+      */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr>              +#+  +:+       +#+         */
/*                                                           +#+#+#+#+#+   +#+            */
/*   Created: 2025/06/04 00:09:03 by aaycan                       #+#    #+#              */
/*   Updated: 2025/06/04 00:09:03 by aaycan                      ###   ########.tr        */
/*                                                                                        */
/* ************************************************************************************** */

#include <unistd.h>
#include <signal.h>
#include <readline/readline.h>

void	handle_sigint(int signum)
{
	if (signum == SIGINT)
	{
		rl_replace_line("", 0);
		write(1, "\n", 1);
		rl_on_new_line();
		rl_redisplay();
	}
}
