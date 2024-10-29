/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsamuel <dsamuel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 17:36:33 by dsamuel           #+#    #+#             */
/*   Updated: 2024/10/29 12:16:26 by dsamuel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * ft_close - Closes a file descriptor if it is valid.
 * 
 * @fd: The file descriptor to be closed.
 * 
 * This function checks if the given file descriptor `fd` is greater than zero, indicating 
 * that it is valid and open. If the condition is met, it closes the file descriptor using 
 * the `close()` system call. This helps to prevent closing invalid or already-closed descriptors.
 */
void ft_close(int fd)
{
    if (fd > 0)
        close(fd);
}

/**
 * reset_std - Resets the standard input and output file descriptors to their original values.
 * 
 * @shell_state: A pointer to the shell state structure (t_mini) containing the original standard descriptors.
 * 
 * This function restores the standard input (STDIN) and standard output (STDOUT) file descriptors 
 * to their original values stored in the `mini` structure. It uses `dup2()` to duplicate the stored 
 * descriptors (`mini->in` and `mini->out`) into the standard input and output descriptors.
 */
void ft_reset_std(t_shell_state *shell_state)
{
    dup2(shell_state->stdin_fd, STDIN);
    dup2(shell_state->stdout_fd, STDOUT);
}

/**
 * close_fds - Closes all file descriptors stored in the shell state structure.
 * 
 * @shell_state: A pointer to the shell state structure (t_mini) containing the file descriptors.
 * 
 * This function calls `ft_close()` for each of the file descriptors stored in the `mini` structure, 
 * including `fdin`, `fdout`, `pipin`, and `pipout`. It ensures that all open file descriptors are 
 * properly closed to prevent resource leaks.
 */
void ft_close_fds(t_shell_state *shell_state)
{
    ft_close(shell_state->input_fd);
    ft_close(shell_state->output_fd);
    ft_close(shell_state->pipe_input_fd);
    ft_close(shell_state->pipe_output_fd);
}

/**
 * reset_fds - Resets all file descriptors and process ID in the shell state structure to their default values.
 * 
 * @mini: A pointer to the shell state structure (t_mini) containing the file descriptors and process ID.
 * 
 * This function sets all file descriptors (`fdin`, `fdout`, `pipin`, `pipout`) in the `mini` structure 
 * to `-1`, indicating that they are uninitialized or closed. It also sets the process ID (`pid`) to `-1`.
 * This helps to indicate that these resources are not currently in use.
 */
void ft_reset_fds(t_shell_state *shell_state)
{
    shell_state->input_fd = -1;
    shell_state->output_fd = -1;
    shell_state->pipe_input_fd = -1;
    shell_state->pipe_output_fd = -1;
    shell_state->proc_id = -1;
}