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
	size_t	g_index;
}t_gc;

void		print_banner(void);
void		handle_sigint(int signum);

t_gc		*init_garbage_collector(void);
void		*gc_malloc(t_gc *gc, size_t size);
void		gc_add(t_gc *gc, void *ptr);
void		gc_collect_all(t_gc *gc);

char		**split_tokens(const char *input, t_gc *garbage_c);
size_t		count_tokens(const char *s);
int			has_unclosed_quotes(const char *s);
char		*strip_quotes(char *s);

t_command	*parse_tokens(char **tokens, t_gc *garbage_c);
t_command	*new_command(t_gc *gc);
int			handle_redirection_token(char **tokens, size_t *i, t_command *cmd);

void		command_executor(t_command *cmd, t_gc *gc, char *line);

size_t		ft_strlen(const char *s);
char		*ft_strndup(const char *s, size_t n);
int			ft_isspace(char c);
int			is_metachar(char c);
int			ft_strcmp(const char *s1, const char *s2);

void		print_commands(t_command *cmd);

#endif