/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_pt_nine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 18:16:49 by aaycan            #+#    #+#             */
/*   Updated: 2025/08/09 18:47:27 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <signal.h>

void	reinstate_shell(t_gc *gc)
{
	signal(SIGINT, handle_sigint_interactive);
	gc_collect_all(gc);
}
