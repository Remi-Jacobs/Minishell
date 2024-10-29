/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_val_level.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsamuel <dsamuel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 15:09:54 by dsamuel           #+#    #+#             */
/*   Updated: 2024/10/29 13:07:46 by dsamuel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * invalid_lvl - Checks if the given string contains only numeric characters.
 * 
 * @str: A pointer to a string representing the shell level.
 * 
 * This function iterates through the given string `str` to verify that it consists 
 * only of numeric characters (digits). It returns 1 if a non-digit character is found, 
 * indicating an invalid level, and 0 otherwise.
 * 
 * Return:
 * - 1 if the string contains a non-numeric character (invalid level).
 * - 0 if the string contains only numeric characters (valid level).
 */
static int ft_invalid_lvl(const char *str)
{
    int i;

    i = 0;
    while (str[i])
    {
        if (!(str[i] >= '0' && str[i] <= '9'))
            return (1);
        i++;
    }
    return (0);
}

/**
 * get_lvl - Parses and retrieves the numeric value from a string representing a shell level.
 * 
 * @str: A pointer to a string representing the shell level.
 * 
 * This function skips any leading whitespace or newline characters in the string `str`. 
 * It checks if the string contains a valid level using the `invalid_lvl` function. If 
 * the string begins with a '+' or '-' sign, it adjusts the parsing accordingly. The function 
 * converts the string into an integer and returns it, applying the appropriate sign if necessary.
 * 
 * Return:
 * - The numeric value of the shell level parsed from the string, considering the sign.
 * - 0 if the level is invalid.
 */

static int ft_get_lvl(const char *str)
{
    int i;
    int sign;
    int num;

    i = 0;
    sign = 1;
    num = 0;
    ft_skip_spacenl(str, &i);
    if (ft_invalid_lvl(str))
        return (0);
    if (str[i] == '-')
        sign = -1;
    if (str[i] == '-' || str[i] == '+')
        i++;
    while (str[i] >= '0' && str[i] <= '9')
        num = num * 10 + (str[i++] - '0');
    return (num * sign);
}

/**
 * increment_shell_level - Increments the shell level (SHLVL) in the environment variable list.
 * 
 * @env: A pointer to the head of the linked list of environment variables (t_env).
 * 
 * This function retrieves the current "SHLVL" value from the environment variable list. 
 * It parses the "SHLVL" value using the `get_lvl` function and increments it by one. The 
 * updated "SHLVL" value is then assigned back to the corresponding environment variable node.
 * 
 * If the "SHLVL" environment variable does not exist or if its value is invalid, 
 * the function exits without making any changes.
 * 
 * How it works:
 * - It uses `get_env_value` to retrieve the current "SHLVL" value from the environment.
 * - If the "SHLVL" value is empty, it returns immediately.
 * - The current "SHLVL" value is converted to an integer using `get_lvl` and incremented by one.
 * - The function iterates through the environment list to find the "SHLVL" variable, 
 *   updating its value with the new incremented level.
 * 
 * Return:
 * - This function does not return a value.
 */
void ft_increment_shell_level(t_env_variable *env)
{
    int sh_lvl_size;
    char env_name[BUFF_SIZE];
    char *sh_lvl;
    char *sh_lvl_val;

    sh_lvl_val = ft_get_env_value("SHLVL", env);
    if (ft_strcmp(sh_lvl_val, "") == 0)
        return ;
    sh_lvl_size = ft_get_lvl(sh_lvl_val) + 1;
    ft_memdel(sh_lvl_val);
    while (env && env->next)
    {
        ft_get_env_name(env_name, env->variable);
        if (ft_strcmp("SHLVL", env_name) == 0)
        {
            ft_memdel(env->variable);
            sh_lvl = ft_itoa(sh_lvl_size);
            env->variable = ft_strjoin("SHLVL=", sh_lvl);
            ft_memdel(sh_lvl);
            return ;
        }
        env = env->next;
    }
}