/* ************************************************************************************** */
/*                                                                                        */
/*                                                                   :::      ::::::::    */
/*   minishell.h                                                   :+:      :+:    :+:    */
/*                                                               +:+ +:+         +:+      */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr>              +#+  +:+       +#+         */
/*                                                           +#+#+#+#+#+   +#+            */
/*   Created: 2025/05/25 18:08:25 by aaycan                       #+#    #+#              */
/*   Updated: 2025/05/25 18:08:25 by aaycan                      ###   ########.tr        */
/*                                                                                        */
/* ************************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#include <stddef.h>

typedef struct s_command
{
	char				**argv;
	char				*infile;
	char				*outfile;
	int					append;
	int					heredoc;
	struct s_command	*next;
}t_command;

typedef struct s_garbage_collector
{
	void	**ptrs;
}t_gc;

void		print_banner(void);
void		handle_sigint(int signum);
char		**split_tokens(const char *input);
size_t		ft_strlen(const char *s);
char		*ft_strndup(const char *s, size_t n);
int			ft_isspace(char c);
int			is_metachar(char c);
t_command	*parse_tokens(char **tokens);
void		command_executor(t_command *cmd);
int			ft_strcmp(const char *s1, const char *s2);

#endif