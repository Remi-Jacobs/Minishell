/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsamuel <dsamuel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 15:58:58 by dsamuel           #+#    #+#             */
/*   Updated: 2024/10/28 18:09:04 by dsamuel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * is_env_char - Checks if a character is a valid environment variable character.
 * 
 * @c: The character to check.
 * 
 * This function determines if the given character `c` is either an alphanumeric 
 * character (letters or digits) or an underscore (`_`), which are valid in environment variable names.
 * 
 * Return:
 * - 1 if the character is a valid environment variable character (alphanumeric or underscore).
 * - 0 otherwise.
 */
int ft_is_env_char(int c)
{
    if (ft_isalnum(c) == 1 || c == '_')
        return (1);
    return (0);
}

/**
 * is_valid_env - Checks if a given string is a valid environment variable name.
 * 
 * @env: A pointer to a string representing an environment variable name.
 * 
 * This function verifies if the string `env` is a valid environment variable name according 
 * to the following rules:
 * - The name must not start with a digit.
 * - The name must consist only of alphanumeric characters or underscores before an equal sign (`=`).
 * 
 * Return:
 * - 0 if the name starts with a digit (invalid name).
 * - -1 if any character in the name (before `=`) is not alphanumeric.
 * - 2 if the name is valid but doesn't have an equal sign (`=`) at the end.
 * - 1 if the name is valid and contains an equal sign (`=`).
 */
int ft_is_valid_env(const char *env)
{
    int i;

    i = 0;
    if (ft_isdigit(env[i]) == 1)
        return (0);
    while (env[i] && env[i] != '=')
    {
        if (ft_isalnum(env[i]) == 0)
            return (-1);
        i++;
    }
    if (env[i] != '=')
        return (2);
    return (1);
}

/**
 * env_value_len - Calculates the length of an environment variable's value.
 * 
 * @env: A pointer to a string representing an environment variable.
 * 
 * This function finds the length of the value portion of an environment variable string, 
 * which starts after the equal sign (`=`). It skips over the name portion to count the 
 * characters in the value.
 * 
 * Return:
 * - The length of the environment variable's value (excluding the equal sign).
 */
int ft_env_value_len(const char *env)
{
    int i;
    int size_name;

    size_name = 0;
    i = 0;
    while (env[i] && env[i] != '=')
        i++;
    i += 1;
    while (env[i])
    {
        i++;
        size_name++;
    }
    return (size_name);
}

/**
 * env_value - Extracts the value portion of an environment variable string.
 * 
 * @env: A pointer to a string representing an environment variable.
 * 
 * This function allocates memory and extracts the value portion from the environment 
 * variable string `env`. It skips over the name portion and equal sign (`=`) to copy 
 * only the value into a newly allocated string.
 * 
 * Return:
 * - A pointer to the newly allocated string containing the value.
 * - NULL if memory allocation fails.
 */
char *ft_env_value(char *env)
{
    int i;
    int j;
    int size_alloc;
    char *env_value;

    size_alloc = ft_env_value_len(env) + 1;
    if (!(env_value = malloc(sizeof(char) * size_alloc)))
        return (NULL);
    i = 0;
    while (env[i] && env[i] != '=')
        i++;
    i += 1;
    j = 0;
    while (env[i])
        env_value[j++] = env[i++];
    env_value[j] = '\0';
    return (env_value);
}

/**
 * get_env_value - Retrieves the value of an environment variable by name.
 * 
 * @arg: A pointer to a string representing the name of the environment variable.
 * @env: A pointer to the head of a linked list of environment variables (t_env).
 * 
 * This function searches through a linked list of environment variables (`env`) for a 
 * variable with a name that matches the `arg` parameter. If found, it extracts and returns 
 * the value of that environment variable. If not found, it returns an empty string.
 * 
 * Return:
 * - A pointer to a newly allocated string containing the value of the environment variable, if found.
 * - An empty string if the environment variable is not found.
 */
char *ft_get_env_value(char *arg, t_env_variable *env)
{
    char env_name[BUFF_SIZE];
    char *env_val;

    env_val = ft_strdup("");
    while (env && env->variable)
    {
        ft_get_env_name(env_name, env->variable);
        if (ft_strcmp(arg, env_name) == 0)
        {
            ft_memdel(env_val);
            env_val = ft_env_value(env->variable);
            return (env_val);
        }
        env = env->next;
    }
    return (env_val);
}