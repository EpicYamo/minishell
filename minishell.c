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

#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

static void	print_banner(void);

int	main(int ac, char **av, char **env)
{
	(void)ac;
	(void)av;
	(void)env;
	print_banner();
	while (1)
    {
        char *line = readline("yamo_shell> ");
        if (!line)
            break;
        if (*line)
		{
			add_history(line);
			printf("%s\n",line);
			if (line[0] == 'e')
			{
				free(line);
				rl_clear_history();
				exit(0);
			}
		}
            
        free(line);
    }
	return (0);
}

static void	print_banner(void)
{
	printf("\033[0;32m");
	printf("░▒▓█▓▒░░▒▓█▓▒░░▒▓███████▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓████████▓▒░▒▓█▓▒░      ░▒▓█▓▒░       \n");
	printf("░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░      ░▒▓█▓▒░      ░▒▓█▓▒░       \n");
	printf("░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░      ░▒▓█▓▒░      ░▒▓█▓▒░       \n");
	printf(" ░▒▓██████▓▒░ ░▒▓██████▓▒░░▒▓████████▓▒░▒▓██████▓▒░ ░▒▓█▓▒░      ░▒▓█▓▒░       \n");
	printf("   ░▒▓█▓▒░          ░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░      ░▒▓█▓▒░      ░▒▓█▓▒░       \n");
	printf("   ░▒▓█▓▒░          ░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░      ░▒▓█▓▒░      ░▒▓█▓▒░       \n");
	printf("   ░▒▓█▓▒░   ░▒▓███████▓▒░░▒▓█▓▒░░▒▓█▓▒░▒▓████████▓▒░▒▓████████▓▒░▒▓████████▓▒░\n");
	printf("\033[0m");
}
