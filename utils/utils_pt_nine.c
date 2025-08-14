/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_pt_nine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 18:16:49 by aaycan            #+#    #+#             */
/*   Updated: 2025/08/14 04:50:49 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <signal.h>
#include <unistd.h>

void	reinstate_shell(t_gc *gc)
{
	signal(SIGINT, handle_sigint_interactive);
	gc_collect_all(gc);
}

void	write_error_with_arg(t_command *cmd)
{
	write(2, "Y-Shell: ", 9);
	write(2, cmd->argv[0], ft_strlen(cmd->argv[0]));
}

int	if_executeable(t_command *cmd)
{
	int	i;

	i = 0;
	while (cmd->argv[0][i])
	{
		if (ft_isspace(cmd->argv[0][i]) != 1)
			break ;
		i++;
	}
	if (cmd->argv[0][i] == '.')
	{
		i++;
		if (cmd->argv[0][i] == '/')
		{
			if (access(cmd->argv[0], X_OK) == 0)
				return (1);
		}
	}
	return (0);
}

t_env	*find_env_node(t_env *env, const char *key)
{
	int		append;
	char	*tmp;

	append = 0;
	tmp = ft_strchr(key, '+');
	if (tmp)
	{
		tmp[0] = '\0';
		append = 1;
	}
	while (env)
	{
		if (env->key && ft_strcmp(env->key, key) == 0)
		{
			if (append == 1)
				tmp[0] = '+';
			return (env);
		}
		env = env->next;
	}
	if (append == 1)
		tmp[0] = '+';
	return (NULL);
}

int	is_valid_identifier(const char *s)
{
	int	i;

	if (!s || !s[0])
		return (0);
	if (!ft_isalpha(s[0]) && (s[0] != '_'))
		return (0);
	i = 1;
	while (s[i] && (s[i] != '='))
	{
		if (!ft_isalnum(s[i]) && (s[i] != '_'))
		{
			if (!((s[i + 1] == '=') && (s[i] == '+')))
				return (0);
		}
		i++;
	}
	return (1);
}
