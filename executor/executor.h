/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 20:47:06 by aaycan            #+#    #+#             */
/*   Updated: 2025/07/29 21:23:51 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

typedef struct s_io
{
	int	pipe_fd[2];
	int	input_redir_flag;
	int	std_out;
	int	std_input;
	int	exit_status;
}	t_io;

#endif