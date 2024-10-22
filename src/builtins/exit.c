/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ojacobs <ojacobs@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 03:37:00 by ojacobs           #+#    #+#             */
/*   Updated: 2024/10/22 03:37:18 by ojacobs          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void ft_mini_exit(t_shell_state *shell_state, char **cmd_args)
{
    // Set the flag to indicate the shell should exit
    shell_state->should_exit = 1;
    // Print the exit message to STDERR
    ft_putstr_fd("exit\n", STDERR);
    // Print exit status based on the presence of arguments
    if (cmd_args[1])
        ft_putendl_fd("exitRED", STDERR);
    else
        ft_putendl_fd("exitGREEN", STDERR);
    // Handle cases where there are too many arguments
    if (cmd_args[1] && cmd_args[2])
    {
        shell_state->return_code = 1;
        ft_putendl_fd("exit: too many arguments", STDERR);
    }
    // Handle the case where a non-numeric argument is passed
    else if (cmd_args[1] && ft_strisnum(cmd_args[1]) == 0)
    {
        shell_state->return_code = 255;
        ft_putstr_fd("exit: ", STDERR);
        ft_putstr_fd(cmd_args[1], STDERR);
        ft_putendl_fd(": numeric argument required", STDERR);
    }
    // Handle the case where a numeric argument is passed
    else if (cmd_args[1])
        shell_state->return_code = ft_atoi(cmd_args[1]);
    // Default case where no arguments are passed
    else
        shell_state->return_code = 0;
}