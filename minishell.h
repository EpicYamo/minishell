/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 03:25:32 by aaycan            #+#    #+#             */
/*   Updated: 2025/06/28 12:59:06 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define EXIT_STATUS_TOKEN "__MINISHELL_EXIT_STATUS__"
# include <stddef.h>

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

size_t		ft_strlen(const char *s);
char		*ft_strndup(const char *s, size_t n);
int			ft_isspace(char c);
int			is_metachar(char c);
int			ft_strcmp(const char *s1, const char *s2);
char		*ft_strchr(const char *s, int c);
char		*ft_strdup(const char *s1);
int			ft_isalnum(int v);
int			is_env_char(char c);
int			ft_isdigit(int v);
int			ft_strncmp(const char *s1, const char *s2, size_t n);
int			ft_atoi(const char *str);

void		print_banner(void);
void		handle_sigint(int signum);

t_gc		*init_garbage_collector(void);
void		*gc_malloc(t_gc *gc, size_t size);
void		gc_add(t_gc *gc, void *ptr);
void		gc_collect_all(t_gc *gc);

char		**split_tokens(const char *input, t_gc *garbage_c);
size_t		count_tokens(const char *s);
int			has_unclosed_quotes(const char *s);
char		*extract_token(const char *s, size_t *i);
char		*strip_quotes(char *s);
char		*expand_env_vars_if_applicable(const char *token, size_t loc);
int			calculate_env_size(const char *token);
void		env_token_exit_status(char *result, const char *token, size_t cursor);
void		env_token_digit(char *result, const char *token, size_t cursor);

t_command	*parse_tokens(char **tokens, t_gc *garbage_c);
t_command	*new_command(t_gc *gc);
int			handle_redirection_token(char **tokens, size_t *i, t_command *cmd, t_gc *gc);
int			handle_heredoc(t_command *cmd, char **tokens, size_t *i, t_gc *gc);

void		command_executor(t_command *cmd, t_gc *gc, char *line);
void		execute_built_in_commands(t_command *cmd, t_gc *gc, char *line);
void		echo_command(t_command *cmd);
void		pwd_command(void);
void		cd_command(t_command *cmd);
void		exit_command(t_command *cmd, t_gc *gc, char *line);

void		print_commands(t_command *cmd);

#endif