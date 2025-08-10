/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment_parser_pt_two.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 16:35:30 by aaycan            #+#    #+#             */
/*   Updated: 2025/08/10 13:54:24 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

static void	create_pwd_env_node(t_env **env_list, t_env **node,
				t_env_error	*err, char *cwd);

void	create_pre_defined_env_nodes(t_env **env_list, t_env **node)
{
	t_env_error	err;
	char		*cwd;

	(*node) = create_env_node("YSHELL=successful", &err);
	if (err == ENV_OK)
		append_env_node(env_list, (*node));
	else if (err == ENV_ALLOC_ERROR)
	{
		write(2, "ENV_ALLOCATION_ERROR\n", 21);
		exit(EXIT_FAILURE);
	}
	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		perror(cwd);
		free_env_list((*env_list));
		exit(EXIT_FAILURE);
	}
	create_pwd_env_node(env_list, node, &err, cwd);
}

static void	create_pwd_env_node(t_env **env_list, t_env **node,
	t_env_error	*err, char *cwd)
{
	char	*pwd_val;

	pwd_val = ft_strjoin("PWD=", cwd);
	free(cwd);
	if (!pwd_val)
	{
		free_env_list((*env_list));
		write(2, "ENV_ALLOCATION_ERROR\n", 21);
		exit(EXIT_FAILURE);
	}
	(*node) = create_env_node(pwd_val, err);
	free(pwd_val);
	if ((*err) == ENV_OK)
		append_env_node(env_list, (*node));
	else if ((*err) == ENV_ALLOC_ERROR)
	{
		free_env_list((*env_list));
		write(2, "ENV_ALLOCATION_ERROR\n", 21);
		exit(EXIT_FAILURE);
	}
}
