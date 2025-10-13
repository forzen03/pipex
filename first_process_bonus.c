/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   first_process_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaradat <njaradat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 15:48:14 by noorjaradat       #+#    #+#             */
/*   Updated: 2025/10/13 14:02:02 by njaradat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

int	heredoc_ops_in(t_doc heredoc, char *cpath, t_program_arg args, int *fd)
{
	int	infd;

	if (heredoc.is_heredoc)
		infd = heredoc.fd_heredoc;
	else
		infd = open(args.argv[1], O_RDONLY);
	if (infd == -1)
	{
		if (fd[1] >= 0)
			close(fd[1]);
		perror("open infile fail");
		free_all(args.cmds);
		ft_free(args.paths, cpath);
	}
	return (infd);
}

void	excute_first(int infd, int *fd, t_program_arg args, char *cpath)
{
	if (dup2(infd, STDIN_FILENO) == -1)
	{
		ft_close(infd, fd[1]);
		perror("dup2 fail in first process");
		ft_free(args.paths, cpath);
	}
	if (dup2(fd[1], STDOUT_FILENO) == -1)
	{
		ft_close(infd, fd[1]);
		perror("dup2 fail in first process");
		ft_free(args.paths, cpath);
	}
	ft_close(infd, fd[1]);
	free_all(args.paths);
	if (args.is_path_cmd && (access(args.cmds[0], X_OK) == 0))
	{
		execve(args.cmds[0], args.cmds, args.env);
		perror("execve fail");
		ft_free(args.cmds, cpath);
	}
	execve(cpath, args.cmds, args.env);
	perror("execve fail");
	ft_free(args.cmds, cpath);
}

void	ft_exit_free(t_program_arg args, int *fd)
{
	ft_exit(-1, "correct path fail", fd[1]);
	if (args.cmds)
		free_all(args.cmds);
	ft_free(args.paths, NULL);
}

void	initial(char **argv, char **env, t_program_arg *args)
{
	args->argv = argv;
	args->env = env;
	args->is_path_cmd = 0;
}

void	first_process(char **argv, t_doc heredoc, char **env, int *fd)
{
	char			*cpath;
	int				infd;
	t_program_arg	args;

	initial(argv, env, &args);
	if (fd[0] >= 0)
		close(fd[0]);
	args.paths = create_paths(args.env);
	if (!args.paths)
		ft_exit(1, "paths fail", fd[1]);
	if (heredoc.is_heredoc)
		args.cmds = ft_split(args.argv[3], ' ');
	else
		args.cmds = ft_split(args.argv[2], ' ');
	if (!args.cmds)
		cleanup_and_exit(args.paths, NULL, "split cmds fail", fd[1]);
	if (args.cmds[0][0] == '/' || args.cmds[0][0] == '.')
		args.is_path_cmd = 1;
	cpath = get_correct_path(args.paths, args.cmds[0], args.is_path_cmd);
	if (!cpath)
		cleanup_and_exit(args.paths, args.cmds, "correct path fail", fd[1]);
	infd = heredoc_ops_in(heredoc, cpath, args, fd);
	excute_first(infd, fd, args, cpath);
}
