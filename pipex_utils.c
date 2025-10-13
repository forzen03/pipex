/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaradat <njaradat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 16:27:46 by noorjaradat       #+#    #+#             */
/*   Updated: 2025/10/13 13:56:45 by njaradat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*get_path(char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], "PATH=", 5) == 0)
			return (env[i]);
		i++;
	}
	return (NULL);
}

char	*complete_path(char *path, char *cmd)
{
	char	*s;
	char	*temp;

	temp = ft_strjoin(path, "/");
	if (!temp)
		return (NULL);
	s = ft_strjoin(temp, cmd);
	if (!s)
	{
		free(temp);
		return (NULL);
	}
	free(temp);
	return (s);
}

char	*get_correct_path(char **paths, char *cmd)
{
	char	**splited_cmds;
	int		i;
	char	*full_path;

	i = 0;
	splited_cmds = ft_split(cmd, ' ');
	if (!splited_cmds)
		return (NULL);
	while (paths[i])
	{
		full_path = complete_path(paths[i], splited_cmds[0]);
		if (!full_path)
			break ;
		if (access(full_path, X_OK) == 0)
		{
			free_all(splited_cmds);
			return (full_path);
		}
		i++;
		free(full_path);
	}
	free_all(splited_cmds);
	return (NULL);
}

void	first_child_excute(char **argv, char **env, char **paths)
{
	char	*cpath;
	char	**args;

	args = ft_split(argv[2], ' ');
	if (!args)
	{
		perror("args split fail");
		ft_free(paths, NULL);
	}
	if ((argv[2][0] == '/' || argv[2][0] == '.') && access(args[0], X_OK) == 0)
		execve(args[0], args, env);
	cpath = get_correct_path(paths, argv[2]);
	if (!cpath)
	{
		perror("command not found");
		free_all(args);
		ft_free(paths, NULL);
	}
	execve(cpath, args, env);
	perror("execve fail in first process");
	free_all(args);
	ft_free(paths, cpath);
}

char	**create_paths(char **env)
{
	char	*env_path;
	char	**paths;

	env_path = get_path(env);
	if (!env_path)
		return (NULL);
	paths = ft_split(env_path + 5, ':');
	return (paths);
}
