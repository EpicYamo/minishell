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

int	main(void)
{
	char	*line;
	char	**tokens;

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
        if (*line)
			add_history(line);
		tokens = split_tokens(line);
		if (tokens)
		{
			for (int i = 0; tokens[i]; i++)
				printf("[%s]\n", tokens[i]);
		}
        free(line);
    }
	return (0);
}
