/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_pt_ten.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 20:54:58 by aaycan            #+#    #+#             */
/*   Updated: 2025/08/27 00:08:20 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <unistd.h>

void	put_newline_and_set_flag(int *flag)
{
	write(1, "\n", 1);
	*flag = 1;
}

void	check_no_newline_option(t_command *cmd, int *newline, int *i)
{
	int	j;

	while (cmd->argv[(*i)])
	{
		if (cmd->argv[(*i)][0] == '-')
		{
			j = 1;
			if (cmd->argv[(*i)][j] == 'n')
			{
				while (cmd->argv[(*i)][j] == 'n')
					j++;
			}
			else
				break ;
			if (cmd->argv[(*i)][j] == '\0')
			{
				(*i)++;
				*newline = 0;
			}
			else
				break ;
		}
		else
			break ;
	}
}
