/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_pt_two.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 02:34:51 by aaycan            #+#    #+#             */
/*   Updated: 2025/08/09 17:30:19 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <unistd.h>

static int	validate_redirection_syntax(char **tokens, size_t *i);
static int	handle_redirection_token_pt_two(char **tokens, t_command *cmd,
				t_gc *gc, t_parser_cursor *cursor);

int	handle_redirection_token(char **tokens, t_command *cmd,
	t_gc *gc, t_parser_cursor *cursor)
{
	int	val;

	cmd->io = cursor->io;
	if (validate_redirection_syntax(tokens, &cursor->i) == -1)
		return (-1);
	if (ft_strcmp(tokens[(*cursor).i], "<") == 0)
	{
		if (check_status_sign_existance(tokens[(*cursor).i + 1]) != 0)
		{
			if (apply_status_sign(&(tokens[(*cursor).i + 1]), cmd, gc) != 0)
				return (-1);
		}
		val = assign_redirection(cmd, tokens, &cursor->i, 0);
		return (val);
	}
	val = handle_redirection_token_pt_two(tokens, cmd, gc, cursor);
	if (val != 0)
		return (val);
	if (ft_strcmp(tokens[(*cursor).i], "<<") == 0)
	{
		val = handle_heredoc(cmd, tokens, &cursor->i, gc);
		return (val);
	}
	return (0);
}

static int	handle_redirection_token_pt_two(char **tokens, t_command *cmd,
	t_gc *gc, t_parser_cursor *cursor)
{
	int	val;

	if (ft_strcmp(tokens[(*cursor).i], ">") == 0)
	{
		if (check_status_sign_existance(tokens[(*cursor).i + 1]) != 0)
		{
			if (apply_status_sign(&(tokens[(*cursor).i + 1]), cmd, gc) != 0)
				return (-1);
		}
		val = assign_redirection(cmd, tokens, &cursor->i, 1);
		return (val);
	}
	else if (ft_strcmp(tokens[(*cursor).i], ">>") == 0)
	{
		if (check_status_sign_existance(tokens[(*cursor).i + 1]) != 0)
		{
			if (apply_status_sign(&(tokens[(*cursor).i + 1]), cmd, gc) != 0)
				return (-1);
		}
		val = assign_redirection(cmd, tokens, &cursor->i, 2);
		return (val);
	}
	return (0);
}

static int	validate_redirection_syntax(char **tokens, size_t *i)
{
	if ((ft_strcmp(tokens[*i], "<") == 0
			|| ft_strcmp(tokens[*i], ">") == 0
			|| ft_strcmp(tokens[*i], "<<") == 0
			|| ft_strcmp(tokens[*i], ">>") == 0))
	{
		if (tokens[*i + 1] == NULL)
		{
			write(2, "Y-Shell: syntax error near", 26);
			write(2, " unexpected token `newline\'\n", 28);
			return (-1);
		}
		else if (ft_strcmp(tokens[*i + 1], "<") == 0
			|| ft_strcmp(tokens[*i + 1], ">") == 0
			|| ft_strcmp(tokens[*i + 1], "<<") == 0
			|| ft_strcmp(tokens[*i + 1], ">>") == 0
			|| ft_strcmp(tokens[*i + 1], "|") == 0)
		{
			write(2, "Y-Shell: syntax error near unexpected token `", 45);
			write(2, tokens[*i + 1], ft_strlen(tokens[*i + 1]));
			write(2, "\'\n", 2);
			return (-1);
		}
	}
	return (0);
}
