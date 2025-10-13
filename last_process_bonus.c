/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   last_process_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaradat <njaradat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 15:50:10 by noorjaradat       #+#    #+#             */
/*   Updated: 2025/10/13 14:02:22 by njaradat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

int	heredoc_ops_out(t_doc heredoc, t_program_arg args, int *fd, char *cpath)
{
	int	outfd;
	int	argc;

	argc = get_argc(args.argv);
	if (heredoc.is_heredoc)
		outfd = open(args.argv[argc - 1], O_RDWR | O_CREAT | O_APPEND, 0777);
	else
		outfd = open(args.argv[argc - 1], O_RDWR | O_CREAT | O_TRUNC, 0777);
	if (outfd == -1)
	{
		ft_exit(0, "open outfile fail", fd[0]);
		free_all(args.cmds);
		ft_free(args.paths, cpath);
	}
	return (outfd);
}

void	excute_last(int outfd, int *fd, t_program_arg args, char *cpath)
{
	if (dup2(outfd, STDOUT_FILENO) == -1)
	{
		ft_close(outfd, fd[0]);
		perror("dup2 fail in last process");
		ft_free(args.paths, cpath);
	}
	if (dup2(fd[0], STDIN_FILENO) == -1)
	{
		ft_close(outfd, fd[0]);
		perror("dup2 fail in last process");
		ft_free(args.paths, cpath);
	}
	ft_close(fd[0], outfd);
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

void	cleanup_and_exit(char **paths, char **cmds, char *msg, int fd)
{
	if (paths)
		free_all(paths);
	if (cmds)
		free_all (cmds);
	ft_exit(1, msg, fd);
}

void	last_process(char **argv, char **env, int *fd, t_doc heredoc)
{
	char			*cpath;
	int				outfd;
	int				argc;
	t_program_arg	args;

	args.argv = argv;
	args.env = env;
	args.is_path_cmd = 0;
	argc = get_argc(argv);
	if (fd[1] >= 0)
		close(fd[1]);
	args.paths = create_paths(env);
	if (!args.paths)
		ft_exit(1, "paths fail", fd[0]);
	args.cmds = ft_split(argv[argc - 2], ' ');
	if (!args.cmds)
		cleanup_and_exit(args.paths, NULL, "split cmds fail", fd[0]);
	if (args.cmds[0][0] == '/' || args.cmds[0][0] == '.')
		args.is_path_cmd = 1;
	cpath = get_correct_path(args.paths, args.cmds[0], args.is_path_cmd);
	if (!cpath)
		cleanup_and_exit(args.paths, args.cmds, "correct path fail", fd[0]);
	outfd = heredoc_ops_out(heredoc, args, fd, cpath);
	excute_last(outfd, fd, args, cpath);
}
