/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsamuel <dsamuel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 20:44:20 by dsamuel           #+#    #+#             */
/*   Updated: 2024/10/22 21:11:47 by dsamuel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * varlcpy - Copies the environment variable value into the new argument string.
 * 
 * @new_arg: A pointer to the newly allocated string where the variable value should be copied.
 * @env_value: The value of the environment variable to be copied.
 * @pos: The current position in the destination string where the copying should start.
 * 
 * This helper function copies the content of `env_value` into `new_arg` starting at the specified 
 * position `pos`. It iterates through `env_value` and updates `new_arg` accordingly. The function 
 * returns the number of characters copied.
 * 
 * Return:
 * - The number of characters copied from `env_value` into `new_arg`.
 */
static int var_val_cpy(char *new_arg, const char *env_value, int pos)
{
    int i;

    i = 0;
    while (env_value[i])
        new_arg[pos++] = env_value[i++];
    return (i);
}

/**
 * insert_var - Inserts an environment variable value into the expanded argument string.
 * 
 * @ex: A pointer to the structure that holds information for the current expansion operation.
 * @arg: The original argument string containing the variable markers.
 * @env: A pointer to the linked list of environment variables (t_env).
 * @ret: The return code to be used for special variable expansion (e.g., `$?`).
 * 
 * This function inserts the value of the environment variable identified within `arg` into 
 * the expanded argument string (`ex->new_arg`). It uses the function `get_var_value()` to retrieve 
 * the variable value. It handles special cases such as the variable `$?` (return code) and numeric 
 * variable identifiers. After inserting the value, it updates the index pointers in the structure 
 * to continue parsing.
 */
// static void insert_var(t_expand_data *data, char *arg, t_env_variable *env, int ret)
// {
//     char *env_value;

//     env_value = get_var_value(arg, data->dest_index, env, ret);
//     data->src_index += env_value ? var_val_cpy(data->expanded_str, env_value, data->src_index) : 0;
//     ft_memdel(env_value);
//     arg[data->dest_index] == '?' ? data->dest_index++ : 0;
//     if (ft_isdigit(arg[data->dest_index]) == 0 && arg[data->dest_index - 1] != '?')
//     {
//         while (is_env_char(arg[data->dest_index]) == 1)
//             data->dest_index++;
//     }
//     else
//     {
//         if (arg[data->dest_index - 1] != '?')
//             data->dest_index++;
//     }
// }

static void insert_var(t_expand_data *data, char *arg, t_env_variable *env, int ret)
{
    char *env_value;

    env_value = get_var_value(arg, data->dest_index, env, ret);
    if (env_value)
        data->src_index += var_val_cpy(data->expanded_str, env_value, data->src_index);
    else
        data->src_index += 0;
    ft_memdel(env_value);
    if (arg[data->dest_index] == '?')
        data->dest_index++;
    if (ft_isdigit(arg[data->dest_index]) == 0 && arg[data->dest_index - 1] != '?')
    {
        while (is_env_char(arg[data->dest_index]) == 1)
            data->dest_index++;
    }
    else
    {
        if (arg[data->dest_index - 1] != '?')
            data->dest_index++;
    }
}

/**
 * expansions - Expands environment variables within a given string.
 * 
 * @arg: A pointer to the original string containing variable markers to be expanded.
 * @env: A pointer to the linked list of environment variables (t_env).
 * @ret: The return code used for expanding special variables such as `$?`.
 * 
 * This function expands environment variables found in the input string `arg`. It allocates 
 * memory for the expanded string based on the estimated length calculated by `arg_alloc_len()`. 
 * The function parses through `arg`, identifying variable markers (`$`), and replaces them with 
 * corresponding values from the environment or the return code. It handles special cases such as 
 * numeric variable identifiers and the `$?` marker.
 * 
 * Return:
 * - A pointer to the newly allocated and expanded string.
 * - NULL if memory allocation fails.
 */

char *expansions(char *arg, t_env_variable *env, int ret)
{
    t_expand_data data;
    int new_arg_len;

    new_arg_len = arg_alloc_len(arg, env, ret);
    if (!(data.expanded_str = malloc(sizeof(char) * new_arg_len + 1)))
        return NULL;
    data.src_index = 0;
    data.dest_index = 0;

    while (data.src_index < new_arg_len && arg[data.dest_index])
    {
        while (arg[data.dest_index] == EXPANSION)
        {
            data.dest_index++;
            if ((arg[data.dest_index] == '\0' || ft_isalnum(arg[data.dest_index]) == 0) && arg[data.dest_index] != '?')
                data.expanded_str[data.src_index++] = '$';
            else
                insert_var(&data, arg, env, ret);
        }
        data.expanded_str[data.src_index++] = arg[data.dest_index++];
    }
    data.expanded_str[data.src_index] = '\0';
    return data.expanded_str;
}
// char *expansions(char *arg, t_env_variable *env, int ret)
// {
//     t_expand_data data;
//     int new_arg_len;

//     new_arg_len = arg_alloc_len(arg, env, ret);
//     if (!(data.expanded_str = malloc(sizeof(char) * new_arg_len + 1)))
//         return (NULL);
//     data.src_index = 0;
//     data.dest_index = 0;
//     while (data.src_index < new_arg_len && arg[data.dest_index])
//     {
//         while (arg[data.dest_index] == EXPANSION)
//         {
//             data.dest_index++;
//             if ((arg[data.dest_index] == '\0' || ft_isalnum(arg[data.dest_index]) == 0) && arg[data.dest_index] != '?')
//                 data.expanded_str[data.src_index++] = '$';
//             else
//                 insert_var(&data, arg, env, ret);
//         }
//         data.expanded_str[data.src_index++] = arg[data.dest_index++];
//     }
//     data.expanded_str[data.dest_index] = '\0';
//     return (data.expanded_str);
// }
