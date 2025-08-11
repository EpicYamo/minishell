/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment_parser_pt_three.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 17:14:48 by aaycan            #+#    #+#             */
/*   Updated: 2025/08/11 17:47:55 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdlib.h>
#include <unistd.h>

static int	calculate_shell_level_value(char **envp);
static void	create_shell_level_str(int shell_level, char **shell_level_str,
				t_env **env_list);

void	create_shell_level(t_env **env_list, t_env **node, char **envp)
{
	t_env_error	err;
	char		*shell_level_str;

	create_shell_level_str(calculate_shell_level_value(envp),
		&shell_level_str, env_list);
	(*node) = create_env_node(shell_level_str, &err);
	free(shell_level_str);
	if (err == ENV_OK)
		append_env_node(env_list, (*node));
	else if (err == ENV_ALLOC_ERROR)
	{
		free_env_list((*env_list));
		write(2, "ENV_ALLOCATION_ERROR\n", 21);
		exit(EXIT_FAILURE);
	}
}

static int	calculate_shell_level_value(char **envp)
{
	int	shell_level;
	int	i;
	int	found_shell_level;

	i = 0;
	found_shell_level = 1;
	while (envp[i])
	{
		found_shell_level = ft_strncmp(envp[i], "SHLVL", 5);
		if (found_shell_level == 0)
			break ;
		i++;
	}
	if (found_shell_level == 0)
		shell_level = ft_atoi(&(envp[i][6])) + 1;
	else
		shell_level = 1;
	return (shell_level);
}

static void	create_shell_level_str(int shell_level, char **shell_level_str,
	t_env **env_list)
{
	char	*val_str;

	val_str = ft_itoa(shell_level);
	if (!val_str)
	{
		free_env_list((*env_list));
		write(2, "ENV_ALLOCATION_ERROR\n", 21);
		exit(EXIT_FAILURE);
	}
	(*shell_level_str) = ft_strjoin("SHLVL=", val_str);
	free(val_str);
	if (!(*shell_level_str))
	{
		free_env_list((*env_list));
		write(2, "ENV_ALLOCATION_ERROR\n", 21);
		exit(EXIT_FAILURE);
	}
}
