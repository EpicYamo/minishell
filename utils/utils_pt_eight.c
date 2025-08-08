/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_pt_eight.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 01:08:37 by aaycan            #+#    #+#             */
/*   Updated: 2025/08/08 21:37:36 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdlib.h>
#include <unistd.h>
#include <readline/history.h>

static char	*ft_char(char *s, unsigned int number, long len)
{
	while (number > 0)
	{
		s[len--] = 48 + (number % 10);
		number = number / 10;
	}
	return (s);
}

static long	ft_num_len(int n)
{
	long	len;

	len = 0;
	if (n <= 0)
		len = 1;
	while (n != 0)
	{
		len++;
		n = n / 10;
	}
	return (len);
}

char	*ft_itoa(int n)
{
	char				*s;
	long				len;
	unsigned int		number;

	len = ft_num_len(n);
	s = (char *)malloc(sizeof(char) * (len + 1));
	if (!(s))
		return (NULL);
	s[len--] = '\0';
	if (n == 0)
		s[0] = '0';
	if (n < 0)
	{
		number = n * -1;
		s[0] = '-';
	}
	else
		number = n;
	s = ft_char(s, number, len);
	return (s);
}

void	write_identifier_error(char *argv)
{
	write(2, "export: `", 9);
	if (argv != NULL)
		write(2, argv, ft_strlen(argv));
	write(2, "`: not a valid identifier\n", 26);
}

int	init_garbage_collector_safe(t_gc **garbage_c, char *line,
	int *exit_status, t_io *shell_io)
{
	add_history(line);
	(*shell_io).exit_status = (*exit_status);
	(*shell_io).exit_stat_ptr = exit_status;
	(*garbage_c) = init_garbage_collector(line);
	if (!(*garbage_c))
	{
		(*exit_status) = 2;
		return (1);
	}
	return (0);
}
