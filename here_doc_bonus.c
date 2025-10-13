/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaradat <njaradat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 14:59:48 by noorjaradat       #+#    #+#             */
/*   Updated: 2025/10/11 18:21:58 by njaradat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	ft_exit(int exit_state, char *msg, int fd)
{
	perror(msg);
	if (fd >= 0)
		close(fd);
	if (exit_state == 1)
		exit(1);
}

int	here_doc(char *limiter)
{
	int		pipe_fd[2];
	char	*line;

	if (pipe(pipe_fd) == -1)
		ft_exit(1, "pipe fail", -1);
	while (1)
	{
		write(2, ">", 1);
		line = get_next_line(0);
		if (!line)
			break ;
		if (ft_strlen(line) > 0 && line[ft_strlen(line) - 1] == '\n')
			line[ft_strlen(line) - 1] = '\0';
		if (ft_strcmp(line, limiter) == 0)
			break ;
		write(pipe_fd[1], line, ft_strlen(line));
		write(pipe_fd[1], "\n", 1);
		free(line);
	}
	if (line)
		free(line);
	if (pipe_fd[1] >= 0)
		close(pipe_fd[1]);
	return (pipe_fd[0]);
}
