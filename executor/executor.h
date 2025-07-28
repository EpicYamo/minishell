/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 20:47:06 by aaycan            #+#    #+#             */
/*   Updated: 2025/07/28 21:18:23 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

typedef struct s_io
{
	int	pipe_fd[2];
	int	prev_fd;
	int	original_stdout;
	int	original_stdin;
	int	built_in_io_flag;
	int	exit_status;
}	t_io;

#endif