/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaradat <njaradat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 19:14:21 by njaradat          #+#    #+#             */
/*   Updated: 2025/10/11 19:30:42 by njaradat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

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

int	ft_free(char **s1, char *s2)
{
	if (s1)
		free_all(s1);
	if (s2)
		free(s2);
	exit(100);
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

char	*get_correct_path(char **paths, char *cmd, int is_path)
{
	int		i;
	char	*full_path;

	i = 0;
	if (is_path)
		return (cmd);
	while (paths[i])
	{
		full_path = complete_path(paths[i], cmd);
		if (!full_path)
			break ;
		if (access(full_path, X_OK) == 0)
			return (full_path);
		i++;
		free(full_path);
	}
	return (NULL);
}
