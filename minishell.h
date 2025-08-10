/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 03:25:32 by aaycan            #+#    #+#             */
/*   Updated: 2025/08/10 21:15:15 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define EXIT_STATUS_TOKEN "--ENCP0MAClAKDJV_MINISHELL_EXIT_STATUS_ENC--"
# define DOLLAR_SIGN_TOKEN "--ENCP0MAClAKDJV_DOLLAR_SIGN_ENC--"
# include <stddef.h>

typedef struct s_io
{
	int		pipe_fd[2];
	int		prev_fd;
	int		original_stdin;
	int		original_stdout;
	int		exit_status;
	int		*pids;
	int		proc_count;
	int		*exit_stat_ptr;
	char	*heredoc_path;
}	t_io;

typedef struct s_command
{
	char				**argv;
	char				*infile;
	char				*outfile;
	int					append;
	int					heredoc;
	int					expand_heredoc;
	t_io				*io;
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
	t_io	*io;
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
	size_t	end_loc;
}	t_lexer_data;

typedef struct s_env_var_bounds
{
	size_t	start_loc;
	size_t	end_loc;
}	t_env_bounds;

typedef struct s_interpret_flag_set
{
	size_t	*flag_set;
	size_t	token_count;
}	t_interpret;

typedef struct s_interpret_data_and_input
{
	t_interpret	interpret_data;
	const char	*line;
}	t_interpret_data_input;

typedef struct s_interpret_data_and_cursor
{
	t_interpret	interpret_data;
	size_t		*i;
}	t_interpret_data_cursor;

typedef struct s_interpret_data_and_tokens
{
	t_interpret	interpret_set;
	char		**tokens;
}	t_interpret_data_tokens;

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
char		*ft_itoa(int n);
int			ft_isalpha(int v);
int			search_str_in_str(const char *haystack, const char *needle);
char		*ft_strjoin(char const *s1, char const *s2);
char		**ft_split(char const *s, char c);
void		*ft_calloc(size_t count, size_t size);
char		*get_minishell_env(char *key, t_env *env_list);
void		free_string_array(char **str_arr);
int			env_list_size(t_env *env);
char		**get_envp(t_env *env_list);
char		*resolve_path(const char *cmd, t_env *env_list);
void		print_banner(void);
void		handle_sigint_interactive(int signum);
t_env		*create_env_list(char **envp);
void		free_env_list(t_env *list);
void		append_env_node(t_env **list, t_env *new_node);
t_gc		*init_garbage_collector(char *line);
void		*gc_malloc(t_gc *gc, size_t size);
void		gc_add(t_gc *gc, void *ptr);
void		gc_collect_all(t_gc *gc);
char		**split_tokens(const char *input, t_gc *garbage_c, t_env *env_list,
				t_interpret interpret_set);
size_t		count_tokens(const char *s);
int			has_unclosed_quotes(const char *s);
char		*extract_token(const char *s, size_t *i);
char		*strip_quotes(char *s, t_interpret interpret_data, size_t index);
char		*expand_env_vars_if_applicable(const char *token, size_t loc,
				t_env *env_list, size_t last_sign);
int			calculate_env_size(const char *token, t_env *env_list, size_t loc);
size_t		check_dollar_sign_existance(char *token, size_t *last_sign);
int			env_token_default(char *result, const char *token, t_env *env_list,
				t_lexer_data data);
void		env_token_exit_status(char *result, const char *token,
				size_t cursor);
void		env_token_digit(char *result, const char *token, size_t cursor);
void		no_valid_env_token(char *result, const char *token, size_t cursor);
char		*replace_dollar_signs(char *token, t_gc *gc);
t_command	*parse_tokens(char **tokens, t_gc *garbage_c, t_io *io,
				t_interpret interpret_set);
t_command	*new_command(t_gc *gc);
int			handle_redirection_token(char **tokens, t_command *cmd, t_gc *gc,
				t_parser_cursor *cursor);
int			handle_heredoc(t_command *cmd, char **tokens, size_t *i, t_gc *gc);
int			command_executor(t_command *cmd, t_gc *gc, char	**formatted_line,
				t_env **env_list);
int			is_builtin(const char *cmd);
void		execute_built_in_commands(t_command *cmd, t_gc *gc,
				char **formatted_line, t_env **env_list);
void		echo_command(t_command *cmd);
void		pwd_command(t_command *cmd);
void		cd_command(t_command *cmd, t_env *env_list);
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
void		execute_non_built_in_command(t_command *cmd, t_env *env_list);
void		exec_built_in_com_in_child_proc(t_command *cmd, t_gc *gc,
				char **formatted_line, t_env **env_list);
void		setup_infile_redirect(t_command *cmd, t_env *env_list);
void		setup_outfile_redirect(t_command *cmd);
int			apply_exit_status_token(t_command *cmd, t_gc *gc);
int			execute_single_built_in_command(t_command **cmd, t_gc *gc,
				char **formatted_line, t_env **env_list);
int			expand_dollar_sign_in_heredoc(t_command *cmd, t_env *env_list);
int			modify_apply_heredoc_file(t_command *cmd, char *infile,
				t_env *env_list);
int			apply_status_sign(char **argv, t_command *cmd, t_gc *gc);
int			check_status_sign_existance(char *argv);
int			skip_command(t_command **cmd);
int			apply_pipe(t_io *io);
void		write_identifier_error(char *argv);
int			assign_redirection(t_command *cmd, char **tokens,
				size_t *i, int type);
int			validate_redirection_file(const char *filename, int type);
int			strip_quotes_and_apply_token(t_gc *gc, char **token,
				t_interpret interpret_data, size_t i);
void		quoted_env_token(char *result, const char *token, size_t cursor);
void		create_pre_defined_env_nodes(t_env **env_list, t_env **node);
t_env		*create_env_node(char *entry, t_env_error *err);
void		handle_sigint_heredoc(int signum);
int			init_garbage_collector_safe(t_gc **garbage_c, char *line,
				int *exit_status, t_io *shell_io);
void		reinstate_shell(t_gc *gc);
int			init_interpret_set(t_gc *gc, t_interpret *interpret_set,
				char *line);
int			handle_pipe_token(char **tokens, t_command **cmd, t_gc *gc,
				t_parser_cursor *cursor);
int			append_token_to_argv(t_command *cmd, char *token, t_gc *gc,
				t_parser_cursor *cursor);

#endif