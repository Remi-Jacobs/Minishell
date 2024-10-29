/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsamuel <dsamuel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 17:24:43 by dsamuel           #+#    #+#             */
/*   Updated: 2024/10/28 18:17:21 by dsamuel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * ret_size - Calculates the number of characters in a return code converted to a string.
 * 
 * @ret: An integer representing the return code.
 * 
 * This function converts the given return code `ret` to a string using `ft_itoa()`, then 
 * calculates its length using `ft_strlen()`. It frees the memory allocated for the string 
 * before returning the length.
 * 
 * Return:
 * - The length of the return code as a string.
 */
int ft_ret_size(int ret)
{
    char *tmp;
    int ret_len;

    tmp = ft_itoa(ret);
    ret_len = ft_strlen(tmp);
    ft_memdel(tmp);
    return (ret_len);
}

/**
 * get_var_len - Calculates the length of an environment variable's value.
 * 
 * @arg: A pointer to the original argument string that contains the variable name.
 * @pos: The current position in the `arg` string where the variable name starts.
 * @env: A pointer to the environment variables (t_env_variable).
 * @ret: An integer representing the return code used for the special variable `$?`.
 * 
 * This function determines the length of an environment variable's value based on its 
 * name. It extracts the variable name from `arg` starting at position `pos` and searches 
 * for its value in the `env` list. If the variable name is `$?`, it returns the length of 
 * the return code string using `ret_size()`. It returns 0 if the variable name is a digit, 
 * indicating an invalid variable. Otherwise, it calculates and returns the length of the 
 * variable's value.
 * 
 * Return:
 * - The length of the environment variable's value as an integer.
 */
int ft_get_var_len(const char *arg, int pos, t_env_variable *env, int ret)
{
    char var_name[BUFF_SIZE];
    char *var_value;
    int i;

    i = 0;
    if (arg[pos] == '?')
        return (ft_ret_size(ret));
    if (ft_isdigit(arg[pos]))
        return (0);
    while (arg[pos] && ft_is_env_char(arg[pos]) == 1 && i < BUFF_SIZE)
    {
        var_name[i] = arg[pos];
        pos++;
        i++;
    }
    var_name[i] = '\0';
    var_value = ft_get_env_value(var_name, env);
    i = ft_strlen(var_value);
    ft_memdel(var_value);
    return (i);
}

/**
 * arg_alloc_len - Calculates the total length needed for an expanded argument string.
 * 
 * @arg: A pointer to the original argument string.
 * @env: A pointer to the environment variables (t_env_variable).
 * @ret: An integer representing the return code used for the special variable `$?`.
 * 
 * This function calculates the length required for an expanded version of the given 
 * argument string `arg`. It processes each character in the string, identifies environment 
 * variable markers (e.g., `$`), and computes the length of the corresponding variable values 
 * using `get_var_len()`. It handles special cases such as digits following `$` and invalid 
 * variable names. The final length includes all characters and expanded variable values.
 * 
 * Return:
 * - The total length of the expanded argument string as an integer.
 */
int ft_arg_alloc_len(const char *arg, t_env_variable *env, int ret)
{
    int i;
    int size;

    i = -1;
    size = 0;
    while (arg[++i])
    {
        if (arg[i] == EXPANSION)
        {
            i++;
            if ((arg[i] == '\0' || ft_isalnum(arg[i]) == 0) && arg[i] != '?')
                size++;
            else
                size += ft_get_var_len(arg, i, env, ret);
            if (ft_isdigit(arg[i]) == 0)
            {
                while (arg[i + 1] && ft_is_env_char(arg[i]))
                    i++;
            }
            else
                size--;
        }
        size++;
    }
    return (size);
}

/**
 * get_var_value - Retrieves the value of an environment variable based on its name.
 * 
 * @arg: A pointer to the original argument string that contains the variable name.
 * @pos: The current position in the `arg` string where the variable name starts.
 * @env: A pointer to the environment variables (t_env_variable).
 * @ret: An integer representing the return code used for the special variable `$?`.
 * 
 * This function extracts the name of an environment variable from `arg` starting at 
 * position `pos`. It then searches for and retrieves the corresponding value from the 
 * `env` list. If the variable name is `$?`, it returns the string representation of the 
 * return code. If the variable name is a digit, it returns NULL, indicating an invalid variable.
 * 
 * Return:
 * - A pointer to the string containing the variable's value.
 * - NULL if the variable name is invalid or not found.
 */
char *ft_get_var_value(const char *arg, int pos, t_env_variable *env, int ret)
{
    char var_name[BUFF_SIZE];
    char *var_value;
    int i;

    i = 0;
    if (arg[pos] == '?')
    {
        var_value = ft_itoa(ret);
        return (var_value);
    }
    if (ft_isdigit(arg[pos]))
        return (NULL);
    while (arg[pos] && ft_is_env_char(arg[pos]) == 1 && i < BUFF_SIZE)
    {
        var_name[i] = arg[pos];
        pos++;
        i++;
    }
    var_name[i] = '\0';
    var_value = ft_get_env_value(var_name, env);
    return (var_value);
}