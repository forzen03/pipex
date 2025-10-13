/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mid_process_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaradat <njaradat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 15:51:44 by noorjaradat       #+#    #+#             */
/*   Updated: 2025/10/13 14:03:24 by njaradat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	excute_mid(int fd1, int fd0, t_program_arg args, char *cpath)
{
	if (dup2(fd1, STDOUT_FILENO) == -1)
	{
		ft_close(fd0, fd1);
		perror("dup2 fail in mid process");
		ft_free(args.paths, cpath);
	}
	if (dup2(fd0, STDIN_FILENO) == -1)
	{
		ft_close(fd0, fd1);
		perror("dup2 fail in mid process");
		ft_free(args.paths, cpath);
	}
	ft_close(fd0, fd1);
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

void	close_exit_free(int fd0, int fd1, t_program_arg args)
{
	ft_close(fd0, fd1);
	free_all(args.paths);
	ft_exit(1, "splited cmds fail", -1);
}

void	mid_process(char *cmd, char **env, int fd0, int fd1)
{
	char			*cpath;
	t_program_arg	args;

	args.is_path_cmd = 0;
	args.env = env;
	args.paths = create_paths(args.env);
	if (!args.paths)
	{
		ft_close(fd0, fd1);
		ft_exit(1, "paths fail", -1);
	}
	args.cmds = ft_split(cmd, ' ');
	if (!args.cmds)
		close_exit_free(fd0, fd1, args);
	if (args.cmds[0][0] == '/' || args.cmds[0][0] == '.')
		args.is_path_cmd = 1;
	cpath = get_correct_path(args.paths, args.cmds[0], args.is_path_cmd);
	if (!cpath)
	{
		ft_close(fd0, fd1);
		free_all(args.cmds);
		free_all(args.paths);
		ft_exit(1, "correct path fail", -1);
	}
	excute_mid(fd1, fd0, args, cpath);
}
