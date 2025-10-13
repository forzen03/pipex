/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaradat <njaradat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 13:38:48 by noorjaradat       #+#    #+#             */
/*   Updated: 2025/10/13 13:56:17 by njaradat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "./libft/libft.h"
# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <unistd.h>

char	*get_path(char **env);
char	*complete_path(char *path, char *cmd);
char	*get_correct_path(char **paths, char *cmd);
void	first_child_excute(char **argv, char **env, char **paths);
char	**create_paths(char **env);
int		ft_free(char **s1, char *s2);
void	ft_close(int fd1, int fd2);
#endif
