/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsamuel <dsamuel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 15:15:58 by dsamuel           #+#    #+#             */
/*   Updated: 2024/10/22 19:35:47 by dsamuel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * str_env_len - Calculates the number of strings in an array of environment variables.
 * 
 * @env: A double pointer to an array of strings representing the environment variables.
 * 
 * This function iterates through the array of strings `env` and counts the number of elements present.
 * It stops counting when it encounters a NULL pointer, which indicates the end of the array.
 * 
 * Return:
 * - The number of strings (environment variables) in the array.
 */
int ft_str_env_len(char **env)
{
    int i;

    i = 0;
    while (env[i])
        i++;
    return (i);
}

/**
 * sort_env - Sorts an array of environment variable strings in alphabetical order.
 * 
 * @tab: A double pointer to an array of strings representing the environment variables to be sorted.
 * @env_len: The length of the array of strings.
 * 
 * This function sorts the array `tab` in place using a bubble sort algorithm. It compares each pair 
 * of adjacent strings and swaps them if they are out of order, continuing the process until the entire 
 * array is sorted in ascending alphabetical order. The sorting is based on the result of `ft_strcmp()`.
 * 
 * Return:
 * - This function does not return a value.
 */
void ft_sort_env(char **tab, int env_len)
{
    int ordered;
    int i;
    char *tmp;

    ordered = 0;
    while (tab && ordered == 0)
    {
        ordered = 1;
        i = 0;
        while (i < env_len - 1)
        {
            if (ft_strcmp(tab[i], tab[i + 1]) > 0)
            {
                tmp = tab[i];
                tab[i] = tab[i + 1];
                tab[i + 1] = tmp;
                ordered = 0;
            }
            i++;
        }
        env_len--;
    }
}

/**
 * print_sorted_env - Prints the sorted list of environment variables in the format "declare -x".
 * 
 * @env: A pointer to a linked list of environment variables (t_env).
 * 
 * This function converts the linked list of environment variables `env` into a single string, which is 
 * then split into an array of strings. It sorts this array of strings alphabetically using `sort_env()`. 
 * Each sorted string is printed to the standard output in the format `"declare -x <variable>"`.
 * 
 * How it works:
 * - Converts the linked list `env` into a single string using `env_to_str()`.
 * - Splits the resulting string into an array of strings, where each element represents an environment variable.
 * - Sorts the array of environment variables alphabetically using `sort_env()`.
 * - Prints each sorted environment variable with the prefix `"declare -x "` using `ft_putstr()` and `ft_putendl()`.
 * - Frees the allocated memory for the array of strings using `free_tab()`.
 * 
 * Return:
 * - This function does not return a value.
 */
void ft_print_sorted_env(t_env_variable *env)
{
    int i;
    char **tab;
    char *str_env;

    str_env = ft_env_to_str(env);
    tab = ft_split(str_env, '\n');
    ft_memdel(str_env);
    ft_sort_env(tab, ft_str_env_len(tab));
    i = 0;
    while (tab[i])
    {
        ft_putstr("declare -x ");
        ft_putendl(tab[i]);
        i++;
    }
    ft_free_tab(tab);
}
