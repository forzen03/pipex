/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaradat <njaradat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 19:09:21 by njaradat          #+#    #+#             */
/*   Updated: 2025/10/12 16:53:14 by njaradat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H

# include "./libft/libft.h"
# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <unistd.h>

typedef struct s_doc
{
	int		is_heredoc;
	int		fd_heredoc;
}			t_doc;

typedef struct s_fd
{
	int		pipe_fd[2];
	int		last_in_fd;
}			t_fd;

typedef struct s_program_arg
{
	char	**argv;
	char	**env;
	char	**cmds;
	char	**paths;
	int		is_path_cmd;
}			t_program_arg;

typedef struct s_cmd_index
{
	int		i;
	int		cmd_num;
}			t_cmd_index;

char		*get_path(char **env);
char		**create_paths(char **env);
char		*complete_path(char *path, char *cmd);
char		*get_correct_path(char **paths, char *cmd, int is_path);
int			ft_free(char **s1, char *s2);
int			here_doc(char *limiter);
void		ft_close(int fd1, int fd2);
int			ft_free(char **s1, char *s2);
void		ft_exit(int exit_state, char *msg, int fd);
int			get_argc(char **argv);
// first process :
void		excute_first(int infd, int *fd, t_program_arg args, char *cpath);
void		first_process(char **argv, t_doc heredoc, char **env, int *fd);
int			heredoc_ops_in(t_doc heredoc, char *cpath, t_program_arg args,
				int *fd);
// last process :
void		excute_last(int outfd, int *fd, t_program_arg args, char *cpath);
void		last_process(char **argv, char **env, int *fd, t_doc heredoc);
int			heredoc_ops_out(t_doc heredoc, t_program_arg args, int *fd,
				char *cpath);
// mid process :
void		excute_mid(int fd1, int fd0, t_program_arg args, char *cpath);
void		mid_process(char *cmd, char **env, int fd0, int fd1);

void		cleanup_and_exit(char **paths, char **cmds, char *msg, int fd);

#endif