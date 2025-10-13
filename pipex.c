/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaradat <njaradat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 13:38:29 by noorjaradat       #+#    #+#             */
/*   Updated: 2025/10/13 16:26:19 by njaradat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	first_child(int *fd, char **argv, char **env, char **paths)
{
	int	infile;

	close(fd[0]);
	infile = open(argv[1], O_RDONLY);
	if (infile == -1)
	{
		perror("open infile fail");
		close(fd[1]);
		ft_free(paths, NULL);
	}
	if (dup2(infile, STDIN_FILENO) == -1)
	{
		perror("dup2 fail infile");
		ft_close(infile, fd[1]);
		ft_free(paths, NULL);
	}
	if (dup2(fd[1], STDOUT_FILENO) == -1)
	{
		perror("dup2 fail pipe write");
		ft_close(infile, fd[1]);
		ft_free(paths, NULL);
	}
	ft_close(infile, fd[1]);
	first_child_excute(argv, env, paths);
}

static void	second_child_excute(char **argv, char **env, char **paths)
{
	char	*cpath;
	char	**args;

	args = ft_split(argv[3], ' ');
	if (!args)
	{
		perror("args split fail");
		ft_free(paths, NULL);
	}
	if ((argv[3][0] == '/' || argv[3][0] == '.') && access(args[0], X_OK) == 0)
		execve(args[0], args, env);
	cpath = get_correct_path(paths, argv[3]);
	if (!cpath)
	{
		perror("command not found");
		free_all(args);
		ft_free(paths, NULL);
	}
	execve(cpath, args, env);
	perror("execve fail in second process");
	free_all(paths);
	ft_free(args, cpath);
}

static void	second_child(int *fd, char **argv, char **env, char **paths)
{
	int	outfile;

	close(fd[1]);
	outfile = open(argv[4], O_RDWR | O_CREAT | O_TRUNC, 0777);
	if (outfile == -1)
	{
		perror("open outfile fail");
		close(fd[0]);
		ft_free(paths, NULL);
	}
	if (dup2(fd[0], STDIN_FILENO) == -1)
	{
		perror("dup2 fail pipe read");
		ft_close(outfile, fd[0]);
		ft_free(paths, NULL);
	}
	if (dup2(outfile, STDOUT_FILENO) == -1)
	{
		perror("dup2 fail outfile");
		ft_close(outfile, fd[0]);
		ft_free(paths, NULL);
	}
	ft_close(outfile, fd[0]);
	second_child_excute(argv, env, paths);
}

static void	second_fork(int *fd, char **argv, char **env, char **paths)
{
	int	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("second fork fail");
		ft_free(paths, NULL);
	}
	if (pid == 0)
		second_child(fd, argv, env, paths);
	else
	{
		close(fd[0]);
		close(fd[1]);
		wait(NULL);
		wait(NULL);
		free_all(paths);
	}
}

int	main(int argc, char **argv, char **env)
{
	int		fd[2];
	int		pid;
	char	**paths;

	if (argc != 5)
	{
		write(2, "The args must be five \n", 23);
		return (1);
	}
	paths = create_paths(env);
	if (!paths)
		return (2);
	if (pipe(fd) == -1)
	{
		perror("pipe fail");
		ft_free(paths, NULL);
	}
	pid = fork();
	if (pid == -1)
		ft_free(paths, NULL);
	if (pid == 0)
		first_child(fd, argv, env, paths);
	else
		second_fork(fd, argv, env, paths);
	return (0);
}
