/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsamuel <dsamuel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 19:44:43 by ojacobs           #+#    #+#             */
/*   Updated: 2024/10/22 19:11:49 by dsamuel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_print_error(char **args)
{
	ft_putstr_fd("cd: ", STDERR);
	if (args[2])
		ft_putstr_fd("String not in pwd: ", STDERR);
	else
	{
		ft_putstr_fd(strerror(errno), STDERR);
		ft_putstr_fd(": ", STDERR);
	}
	ft_putendl_fd(args[1], STDERR);
}

static  char  *ft_get_env_path(t_env_variable *env, const char *var, size_t len)
{
	char  *old_pwd;
	int  i;
	int j;
	int s_alloc;

	while (env && env->next != NULL)
	{
		if (ft_strncmp(env->variable, var, len) == 0)
		{
			s_alloc = ft_strlen(env->variable) - len;
			if (!(old_pwd = (char *)malloc(sizeof(char) * (s_alloc + 1))))
				return (NULL);
			i = 0;
			j = 0;
			while (env->variable[i++])
			{
				if (i > (int)len)
					old_pwd[j++] = env->variable[i];
			}
			old_pwd[j] = '\0';
			return (old_pwd);
		}
		env = env->next;
	}
	return (NULL);
}

static int ft_update_old_pwd(t_env_variable *env)
{
	char	cwd[PATH_MAX];
	char	*old_pwd;

	if (getcwd(cwd, PATH_MAX) == NULL )
		return (ERROR);
	if (!(old_pwd = ft_strjoin("OLDPWD=", cwd)))
		return (ERROR);
	if (ft_is_in_env(env, old_pwd) == SUCCESS)
		ft_add_to_env(old_pwd, env);
	ft_memdel(old_pwd);
	return (SUCCESS);
}

static int	ft_go_to_path(int is_old, t_env_variable *env)
{
	int 	return_cd;
	char	*path;

	path = NULL;
	if (is_old == 0)
	{
		ft_update_old_pwd(env);
		path = ft_get_env_path(env, "HOME", 4);
		if (!path)
			ft_putendl_fd("cd: HOME not set", STDERR);
		if (!path)
			return (ERROR);
	}
	else if (is_old == 1)
	{
		path = ft_get_env_path(env, "OLDPWD", 6);
		if (!path)
		{
			ft_putendl_fd("cd: OLDPWD not set", STDERR);
			return (ERROR);
		}
		ft_update_old_pwd(env);
	}
	return_cd = chdir(path);
	ft_memdel(path);
	return (return_cd);
}

int	ft_cd(char **args, t_env_variable *env)
{
	int		return_cd;

	if (!args[1])
		return (ft_go_to_path(0, env));
	if (ft_strcmp(args[1], "-") == 0)
		return_cd = ft_go_to_path(1, env);
	else
	{
		ft_update_old_pwd(env);
		return_cd = chdir(args[1]);
		if (return_cd < 0)
			return_cd *= -1;
		if (return_cd != 0)
			ft_print_error(args);
	}
	return (return_cd);
}

//These are all for the main to test this function. They may eventually be useful for other funtions

// t_env_variable	*ft_load_env(char **envp)
// {
// 	t_env_variable	*env_list;
// 	t_env_variable	*new_node;
// 	t_env_variable	*current;
// 	int				i;

// 	// Initialize the environment list
// 	env_list = NULL;
// 	i = 0;

// 	// Iterate over the environment strings
// 	while (envp[i])
// 	{
// 		// Allocate memory for a new environment variable node
// 		if (!(new_node = (t_env_variable *)malloc(sizeof(t_env_variable))))
// 			return (NULL);  // Handle malloc failure

// 		// Duplicate the environment variable string
// 		new_node->variable = ft_strdup(envp[i]);
// 		new_node->next = NULL;

// 		// If the list is empty, set the new node as the first node
// 		if (!env_list)
// 			env_list = new_node;
// 		else
// 		{
// 			// Find the end of the list and append the new node
// 			current = env_list;
// 			while (current->next)
// 				current = current->next;
// 			current->next = new_node;
// 		}
// 		i++;
// 	}

// 	// Return the populated environment list
// 	return (env_list);
// }

// void	ft_free_env(t_env_variable *env)
// {
// 	t_env_variable	*temp;

// 	while (env)
// 	{
// 		// Store the next node before freeing the current node
// 		temp = env->next;

// 		// Free the environment variable string
// 		ft_memdel(env->variable);

// 		// Free the current node
// 		free(env);

// 		// Move to the next node
// 		env = temp;
// 	}
// }



// int main(int argc, char **argv, char **envp)
// {
//     t_env_variable *env = NULL;

//     // Load the environment variables (you would need an `ft_load_env` function)
//     env = ft_load_env(envp);

//     // Call the `ft_cd` function with command-line arguments to simulate input
//     if (argc > 1)
//     {
//         if (ft_cd(argv, env) == 0) // Call ft_cd with the arguments
//             printf("Changed directory to: %s\n", argv[1]);
//         else
//             printf("Failed to change directory to: %s\n", argv[1]);
//     }
//     else
//     {
//         printf("Usage: %s <directory>\n", argv[0]);
//     }

//     // Free the environment variables list
//     ft_free_env(env);

//     return 0;
// }