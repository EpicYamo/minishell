/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 03:25:32 by aaycan            #+#    #+#             */
/*   Updated: 2025/07/22 18:06:26 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define EXIT_STATUS_TOKEN "__ENCP0MAClAKDJV_MINISHELL_EXIT_STATUS_ENC__"
# define DOLLAR_SIGN_TOKEN "__ENCP0MAClAKDJV_DOLLAR_SIGN_ENC__"
# include <stddef.h>

typedef struct s_command
{
	char				**argv;
	char				*infile;
	char				*outfile;
	int					append;
	int					heredoc;
	struct s_command	*next;
}	t_command;

typedef struct s_garbage_collector
{
	void	**ptrs;
	size_t	g_index;
}	t_gc;

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef enum e_env_error
{
	ENV_OK,
	ENV_SKIP,
	ENV_ALLOC_ERROR
}	t_env_error;

typedef struct s_parser_cursor
{
	size_t	i;
	size_t	argc;
}	t_parser_cursor;

typedef struct s_lexer_cursor
{
	size_t	i;
	size_t	*loc;
}	t_lexer_cursor;

typedef struct s_lexer_data
{
	size_t	cursor;
	size_t	key_size;
}	t_lexer_data;

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
int			ft_isalpha(int v);
int			search_str_in_str(const char *haystack, const char *needle);
char		*ft_strjoin(char const *s1, char const *s2);
char		**ft_split(char const *s, char c);
void		*ft_calloc(size_t count, size_t size);
char		*get_minishell_env(char *key, t_env *env_list);
void		free_string_array(char **str_arr);
void		print_banner(void);
void		handle_sigint(int signum);
t_env		*create_env_list(char **envp);
void		free_env_list(t_env *list);
void		append_env_node(t_env **list, t_env *new_node);
t_gc		*init_garbage_collector(char *line);
void		*gc_malloc(t_gc *gc, size_t size);
void		gc_add(t_gc *gc, void *ptr);
void		gc_collect_all(t_gc *gc);
char		**split_tokens(const char *input, t_gc *garbage_c, t_env *env_list);
size_t		count_tokens(const char *s);
int			has_unclosed_quotes(const char *s);
char		*extract_token(const char *s, size_t *i);
char		*strip_quotes(char *s);
char		*expand_env_vars_if_applicable(const char *token, size_t loc,
				t_env *env_list);
int			calculate_env_size(const char *token, t_env *env_list, size_t loc);
size_t		check_dollar_sign_existance(char *token, size_t *last_sign);
int			env_token_default(char *result, const char *token, t_env *env_list,
				t_lexer_data data);
void		env_token_exit_status(char *result, const char *token,
				size_t cursor);
void		env_token_digit(char *result, const char *token, size_t cursor);
void		no_valid_env_token(char *result, const char *token, size_t cursor);
char		*replace_dollar_signs(char *token, t_gc *gc);
t_command	*parse_tokens(char **tokens, t_gc *garbage_c);
t_command	*new_command(t_gc *gc);
int			handle_redirection_token(char **tokens, t_command *cmd, t_gc *gc,
				t_parser_cursor *cursor);
int			handle_heredoc(t_command *cmd, char **tokens, size_t *i, t_gc *gc);
void		command_executor(t_command *cmd, t_gc *gc, char	**formatted_line,
				t_env *env_list);
int			is_builtin(const char *cmd);
void		execute_built_in_commands(t_command *cmd, t_gc *gc,
				char **formatted_line, t_env *env_list);
void		echo_command(t_command *cmd);
void		pwd_command(t_command *cmd);
void		cd_command(t_command *cmd);
void		exit_command(t_command *cmd, t_gc *gc, char **formatted_line,
				t_env *env_list);
void		env_command(t_command *cmd, t_env *env);
void		export_command(t_command *cmd, t_env **env_list);
void		print_export_list(t_command *cmd, t_env *env);
void		print_export_list_to_stdout(t_env *tmp);
void		print_export_list_to_outfile(t_command *cmd, t_env *tmp);
int			split_key_value(char *arg, char **key, char **value);
void		swap_env_nodes(t_env *a, t_env *b);
void		export_malloc_fail_handler(t_env *copy, t_env *node, int option);
void		unset_command(t_command *cmd, t_env **env_list);
void		print_commands(t_command *cmd);

int			env_list_size(t_env *env);
char		**get_envp(t_env *env_list);
void		ft_free_split(char **split);
char		*get_env_value(t_env *env_list, const char *key);
char		*resolve_path(const char *cmd, t_env *env_list);

#endif