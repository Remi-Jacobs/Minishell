/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_vars.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsamuel <dsamuel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 15:16:19 by dsamuel           #+#    #+#             */
/*   Updated: 2024/10/22 15:25:08 by dsamuel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * size_env - Calculates the total length of all environment variable values in a linked list.
 * 
 * @lst: A pointer to the first node in the linked list of environment variables (t_env).
 * 
 * This function iterates through the linked list `lst` and calculates the total length of all
 * non-NULL environment variable values. Each value's length is obtained using `ft_strlen()`, 
 * and an additional character is counted to account for the separator (newline or null).
 * 
 * Return:
 * - The total length of all environment variable values in the list.
 */
size_t size_env(t_env_variable *lst)
{
    size_t lst_len;

    lst_len = 0;
    while (lst && lst->next != NULL)
    {
        if (lst->variable != NULL)
        {
            lst_len += ft_strlen(lst->variable);
            lst_len++;
        }
        lst = lst->next;
    }
    return (lst_len);
}

/**
 * env_to_str - Converts a linked list of environment variables to a single string.
 * 
 * @lst: A pointer to the first node in the linked list of environment variables (t_env).
 * 
 * This function creates a single string `env` from the linked list of environment variables. 
 * It allocates memory for the string based on the total size calculated by `size_env()`. It 
 * then concatenates all non-NULL environment variable values, separating them with a newline.
 * 
 * Return:
 * - A pointer to the newly allocated string containing all environment variables separated by newlines.
 * - NULL if memory allocation fails.
 */
char *env_to_str(t_env_variable *lst)
{
    char *env;
    int i;
    int j;

    if (!(env = malloc(sizeof(char) * size_env(lst) + 1)))
        return (NULL);
    i = 0;
    while (lst && lst->next != NULL)
    {
        if (lst->variable != NULL)
        {
            j = 0;
            while (lst->variable[j])
            {
                env[i] = lst->variable[j];
                i++;
                j++;
            }
        }
        if (lst->next->next != NULL)
            env[i++] = '\n';
        lst = lst->next;
    }
    env[i] = '\0';
    return (env);
}

/**
 * env_init - Initializes the environment linked list from an array of environment variables.
 * 
 * @shell_state: A pointer to the main shell state structure (t_mini) containing environment information.
 * @env_array: An array of strings representing environment variables.
 * 
 * This function initializes the `mini->env` linked list with values from the `env_array` array. 
 * It allocates memory for each node, duplicates the string, and connects each node to form a linked list.
 * 
 * Return:
 * - 0 if the initialization is successful.
 * - 1 if memory allocation fails.
 */
int env_init(t_shell_state *shell_state, char **env_array)
{
    t_env_variable *env;
    t_env_variable *new;
    int i;

    if (!(env = malloc(sizeof(t_env_variable))))
        return (1);
    env->variable = ft_strdup(env_array[0]);
    env->next = NULL;
    shell_state->active_env = env;
    i = 1;
    while (env_array && env_array[0] && env_array[i])
    {
        if (!(new = malloc(sizeof(t_env_variable))))
            return (1);
        new->variable = ft_strdup(env_array[i]);
        new->next = NULL;
        env->next = new;
        env = new;
        i++;
    }
    return (0);
}

/**
 * secret_env_init - Initializes the secret environment linked list from an array of environment variables.
 * 
 * @shell_array: A pointer to the main shell state structure (t_mini) containing secret environment information.
 * @env_array: An array of strings representing environment variables.
 * 
 * This function initializes the `mini->secret_env` linked list with values from the `env_array` array.
 * It allocates memory for each node, duplicates the string, and connects each node to form a linked list.
 * 
 * Return:
 * - 0 if the initialization is successful.
 * - 1 if memory allocation fails.
 */
int secret_env_init(t_shell_state *shell_state, char **env_array)
{
    t_env_variable *env;
    t_env_variable *new;
    int i;

    if (!(env = malloc(sizeof(t_env_variable))))
        return (1);
    env->variable = ft_strdup(env_array[0]);
    env->next = NULL;
    shell_state->secret_env = env;
    i = 1;
    while (env_array && env_array[0] && env_array[i])
    {
        if (!(new = malloc(sizeof(t_env_variable))))
            return (1);
        new->variable = ft_strdup(env_array[i]);
        new->next = NULL;
        env->next = new;
        env = new;
        i++;
    }
    return (0);
}