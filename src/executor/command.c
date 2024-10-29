/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsamuel <dsamuel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 22:25:18 by ojacobs           #+#    #+#             */
/*   Updated: 2024/10/28 18:14:28 by dsamuel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int			ft_error_message(char *path)
{
	DIR	*folder;
	int	fd;
	int	ret;

	fd = open(path, O_WRONLY);
	folder = opendir(path);
	ft_putstr_fd("minishell: ", STDERR);
	ft_putstr_fd(path, STDERR);
	if (ft_strchr(path, '/') == NULL)
		ft_putendl_fd(": command not found", STDERR);
	else if (fd == -1 && folder == NULL)
		ft_putendl_fd(": No such file or directory", STDERR);
	else if (fd == -1 && folder != NULL)
		ft_putendl_fd(": is a directory", STDERR);
	else if (fd != -1 && folder == NULL)
		ft_putendl_fd(": Permission denied", STDERR);
	if (ft_strchr(path, '/') == NULL || (fd == -1 && folder == NULL))
		ret = UNKNOWN_CMD;
	else
		ret = IS_DIRECTORY;
	if (folder)
		closedir(folder);
	ft_close(fd);
	return (ret);
}

int	ft_magic_box(char *path, char **args, t_env_variable *env, t_shell_state *shell_state)
{
	char	**env_array;
	char	*env_string;
	int		ret;

	ret = SUCCESS;
	global_sig.child_proc_id = fork();

	if (global_sig.child_proc_id == 0)  // Child process
	{
		// Convert environment variables to a string format and split them into an array
		env_string = ft_env_to_str(env);
		env_array = ft_split(env_string, '\n');
		ft_memdel(env_string);

		// Execute command if the path contains a '/'
		if (ft_strchr(path, '/') != NULL)
			execve(path, args, env_array);

		// Handle errors if execve fails
		ret = ft_error_message(path);
		ft_free_tab(env_array);
		ft_free_token(shell_state->cmd_list);
		exit(ret);  // Exit with the error code
	}
	else  // Parent process
	{
		// Wait for the child process to finish and retrieve its exit status
		waitpid(global_sig.child_proc_id, &ret, 0);
	}

	// Check if a signal was received (SIGINT or SIGQUIT) and return the last exit status
	if (global_sig.sigint_received == 1 || global_sig.sigquit_received == 1)
		return (global_sig.last_exit_stat);

	// Normalize the return value based on specific codes
	if (ret == 32256 || ret == 32512)
		ret = ret / 256;
	else
		ret = !!ret;

	return (ret);
}

char		*ft_path_join(const char *s1, const char *s2)
{
	char	*tmp;
	char	*path;

	tmp = ft_strjoin(s1, "/");
	path = ft_strjoin(tmp, s2);
	ft_memdel(tmp);
	return (path);
}

char		*ft_check_dir(char *bin, char *command)
{
	DIR				*folder;
	struct dirent	*item;
	char			*path;

	path = NULL;
	folder = opendir(bin);
	if (!folder)
		return (NULL);
	while ((item = readdir(folder)))
	{
		if (ft_strcmp(item->d_name, command) == 0)
			path = ft_path_join(bin, item->d_name);
	}
	closedir(folder);
	return (path);
}

int			ft_exec_bin(char **args, t_env_variable *env, t_shell_state *shell_state)
{
	int		i;
	char	**bin;
	char	*path;
	int		ret;

	i = 0;
	ret = UNKNOWN_CMD;
	while (env && env->variable && ft_strncmp(env->variable, "PATH=", 5) != 0)
		env = env->next;
	if (env == NULL || env->next == NULL)
		return (ft_magic_box(args[0], args, env, shell_state));
	bin = ft_split(env->variable, ':');
	if (!args[0] && !bin[0])
		return (ERROR);
	i = 1;
	path = ft_check_dir(bin[0] + 5, args[0]);
	while (args[0] && bin[i] && path == NULL)
		path = ft_check_dir(bin[i++], args[0]);
	if (path != NULL)
		ret = ft_magic_box(path, args, env, shell_state);
	else
		ret = ft_magic_box(args[0], args, env, shell_state);
	ft_free_tab(bin);
	ft_memdel(path);
	return (ret);
}
