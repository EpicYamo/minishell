/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_pt_five.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 22:26:09 by aaycan            #+#    #+#             */
/*   Updated: 2025/08/04 19:04:37 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

static int	split_key_value_pt_two(char *arg, char **key, char **value);

int	split_key_value(char *arg, char **key, char **value)
{
	char	*eq;

	eq = ft_strchr(arg, '=');
	if (eq)
	{
		*key = ft_strndup(arg, eq - arg);
		if (!(*key))
		{
			write(2, "ALLOCATION_ERROR for Export Command\n", 36);
			return (1);
		}
		*value = ft_strdup(eq + 1);
		if (!(*value))
		{
			free(key);
			write(2, "ALLOCATION_ERROR for Export Command\n", 36);
			return (1);
		}
	}
	else
	{
		if (split_key_value_pt_two(arg, key, value) != 0)
			return (1);
	}
	return (0);
}

static int	split_key_value_pt_two(char *arg, char **key, char **value)
{
	*value = NULL;
	*key = ft_strdup(arg);
	if (!(*key))
	{
		write(2, "ALLOCATION_ERROR for Export Command\n", 36);
		return (1);
	}
	return (0);
}

void	cd_command(t_command *cmd, t_env *env_list)
{
	char	*home;

	(*cmd->io->exit_stat_ptr) = 1;
	if (cmd->argv[1])
	{
		if (chdir(cmd->argv[1]) != 0)
			perror("cd");
		else
			(*cmd->io->exit_stat_ptr) = 0;
	}
	else
	{
		home = get_minishell_env("HOME", env_list);
		if (!home)
			write(2, "cd: HOME not set\n", 17);
		if ((home != NULL) && (chdir(home) != 0))
			perror("cd");
		else if (home != NULL)
			(*cmd->io->exit_stat_ptr) = 0;
	}
}
