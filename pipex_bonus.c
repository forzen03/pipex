/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaradat <njaradat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 13:35:11 by noorjaradat       #+#    #+#             */
/*   Updated: 2025/10/13 16:31:28 by njaradat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	child_process(t_fd fds, t_doc heredoc, t_program_arg args,
		t_cmd_index cmds)
{
	int	last_pipe[2];

	if (cmds.i == 0)
		first_process(args.argv, heredoc, args.env, fds.pipe_fd);
	else if (cmds.i == cmds.cmd_num - 1)
	{
		last_pipe[0] = fds.last_in_fd;
		last_pipe[1] = -1;
		last_process(args.argv, args.env, last_pipe, heredoc);
	}
	else
	{
		if (fds.pipe_fd[0] >= 0)
			close(fds.pipe_fd[0]);
		if (heredoc.is_heredoc)
			mid_process(args.argv[cmds.i + 3], args.env, fds.last_in_fd,
				fds.pipe_fd[1]);
		else
			mid_process(args.argv[cmds.i + 2], args.env, fds.last_in_fd,
				fds.pipe_fd[1]);
	}
}

void	initialize_vars(t_fd *fds, t_cmd_index *cmds, t_program_arg *args,
		t_doc *heredoc)
{
	cmds->i = -1;
	fds->last_in_fd = -1;
	if (heredoc->is_heredoc)
		cmds->cmd_num = get_argc(args->argv) - 4;
	else
		cmds->cmd_num = get_argc(args->argv) - 3;
}

void	fork_pip(t_cmd_index cmds, t_fd *fds, int *pid)
{
	if (cmds.i < cmds.cmd_num - 1)
	{
		if (pipe(fds->pipe_fd) == -1)
			ft_exit(1, "pipe fail", -1);
	}
	*pid = fork();
	if (*pid == -1)
	{
		ft_close(fds->pipe_fd[0], fds->pipe_fd[1]);
		ft_exit(1, "fork failed", -1);
	}
}

void	processes(char **argv, char **env, t_doc heredoc)
{
	int				pid;
	t_fd			fds;
	t_cmd_index		cmds;
	t_program_arg	args;

	args.argv = argv;
	args.env = env;
	initialize_vars(&fds, &cmds, &args, &heredoc);
	while (++(cmds.i) < cmds.cmd_num)
	{
		fork_pip(cmds, &fds, &pid);
		if (pid == 0)
			child_process(fds, heredoc, args, cmds);
		if (heredoc.is_heredoc && cmds.i == 0)
			ft_close(heredoc.fd_heredoc, -1);
		ft_close(fds.last_in_fd, -1);
		if (cmds.i < cmds.cmd_num - 1)
		{
			if (fds.pipe_fd[1] >= 0)
				close(fds.pipe_fd[1]);
			fds.last_in_fd = fds.pipe_fd[0];
		}
	}
	while (wait(NULL) > 0)
		;
}

int	main(int argc, char **argv, char **env)
{
	t_doc	heredoc;

	heredoc.is_heredoc = 0;
	heredoc.fd_heredoc = -1;
	if (ft_strncmp(argv[1], "here_doc", 8) == 0)
	{
		if (argc < 6)
		{
			write(2, "here_doc LIMITER cmd1 cmd2 ... cmdN outfile\n", 45);
			return (1);
		}
		heredoc.is_heredoc = 1;
		heredoc.fd_heredoc = here_doc(argv[2]);
	}
	else if (argc < 5)
	{
		write(2, "infile cmd1 cmd2 ... cmdN outfile\n", 35);
		return (1);
	}
	processes(argv, env, heredoc);
	if (heredoc.fd_heredoc >= 0)
		close(heredoc.fd_heredoc);
	return (0);
}
