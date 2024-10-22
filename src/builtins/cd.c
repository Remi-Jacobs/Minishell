/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ojacobs <ojacobs@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 19:44:43 by ojacobs           #+#    #+#             */
/*   Updated: 2024/10/21 23:04:34 by ojacobs          ###   ########.fr       */
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

	while (env && env-env->next)
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

char		*ft_get_env_name(char *dest, const char *src)
{
	int		i;

	i = 0;
	while (src[i] && src[i] != '=' && ft_strlen(src) < BUFF_SIZE)
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}


int			ft_is_in_env(t_env_variable *env, char *args)
{
	char	var_name[BUFF_SIZE];
	char	env_name[BUFF_SIZE];

	ft_get_env_name(var_name, args);
	while (env && env->next)
	{
		ft_get_env_name(env_name, env->variable);
		if (ft_strcmp(var_name, env_name) == 0)
		{
			ft_memdel(env->variable);
			env->variable = ft_strdup(args);
			return (1);
		}
		env = env->next;
	}
	return (SUCCESS);
}

//This will update the env seperate from and more appropraitely how ft_add_to_env would have handled it.
int ft_env_update(const char *args, t_env_variable *env)
{
    t_env_variable	*new_env;
    t_env_variable	*current;
    char			var_name[BUFF_SIZE];
    char			env_name[BUFF_SIZE];

    // Extract the variable name from the provided args (before '=')
    ft_get_env_name(var_name, args);

    // Iterate through the environment list to check for an existing variable
    current = env;
    while (current)
    {
        ft_get_env_name(env_name, current->variable);
        if (ft_strcmp(var_name, env_name) == 0)  // If variable exists, update it
        {
            ft_memdel(current->variable);  // Free the old variable
            current->variable = ft_strdup(args);  // Update with new value
            return (SUCCESS);
        }
        current = current->next;
    }

    // If the variable does not exist, create a new one and add it to the end
    if (!(new_env = (t_env_variable *)malloc(sizeof(t_env_variable))))
        return (-1);
    new_env->variable = ft_strdup(args);
    new_env->next = NULL;

    // Find the end of the list to append the new variable
    current = env;
    while (current->next)
        current = current->next;
    current->next = new_env;

    return (SUCCESS);
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
		ft_env_update(old_pwd, env);
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