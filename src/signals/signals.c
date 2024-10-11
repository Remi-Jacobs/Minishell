/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsamuel <dsamuel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 20:00:14 by dsamuel           #+#    #+#             */
/*   Updated: 2024/10/11 21:00:22 by dsamuel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Frees the memory pointed to by `ptr` and sets the pointer to NULL.
 *
 * This function is used to safely free dynamically allocated memory
 * and set the pointer to NULL to avoid dangling pointers.
 * It ensures that `ptr` is not NULL before attempting to free it.
 *
 * @param ptr A pointer to the memory to be freed.
 * @return Always returns NULL, making it easier to reset the pointer.
 *
 * Usage:
 *   char *str = malloc(100);
 *   str = ft_memdel(str);
 *   // str is now NULL after being freed.
 *  we will need to move to libft implementation
 */
void	*ft_memdel(void *ptr)
{
	if (ptr)
	{
		free(ptr);
		ptr = NULL;
	}
	return (NULL);
}

/**
 * @brief Handles the SIGINT signal (Ctrl+C) for the minishell.
 *
 * This function is called when a SIGINT signal is received by the shell.
 * It differentiates between when a child process is running and when the
 * shell is in the foreground without a child process.
 * - If no child process is running (`global_sig.child_proc_id == 0`):
 *   - Prints a new line followed by the shell prompt to `STDERR`.
 *   - Sets the `last_exit_stat` to 1 to indicate an interrupted command.
 * - If a child process is running:
 *   - Prints a newline to indicate the interruption.
 *   - Sets `last_exit_stat` to 130, which is the typical exit code for
 *     a process terminated by SIGINT.
 * - Updates `global_sig.sigint_received` to 1, indicating that a SIGINT
 *   signal has been received.
 *
 * @param signal The signal number (typically SIGINT).
 *               This parameter is unused but included for compatibility.
 */
void	ft_signal_init(int signal)
{
	(void)signal;
	if (global_sig.child_proc_id == 0)
	{
		ft_putstr_fd("\b\b", STDERR);
		ft_putstr_fd("\n", STDERR);
		ft_putstr_fd("\033[0;36m\033[1m minishell ▸ \033[0m", STDERR);
		global_sig.last_exit_stat = 1;
	}
	else
	{
		ft_putstr_fd("\n", STDERR);
		global_sig.last_exit_stat = 130;
	}
	global_sig.sigint_received = 1;
}

/**
 * @brief Handles the SIGQUIT signal (Ctrl+\) for the minishell.
 *
 * This function is called when a SIGQUIT signal is received by the shell.
 * It differentiates between when a child process is running and when the
 * shell is in the foreground without a child process.
 * - Converts the signal number to a string using `ft_itoa` for display.
 * - If a child process is running (`global_sig.child_proc_id != 0`):
 *   - Prints a message indicating the quit signal (`^\\`) to `STDERR`.
 *   - Prints the signal number (131) to indicate the process was
 *     terminated by SIGQUIT.
 *   - Sets `last_exit_stat` to 131, which is the typical exit code for
 *     a process terminated by SIGQUIT.
 *   - Sets `sigquit_received` to 1 to indicate that SIGQUIT was received.
 * - If no child process is running, it suppresses the default output.
 * - Frees the memory allocated for the signal number string using `ft_memdel`.
 *
 * @param signal The signal number (typically SIGQUIT).
 */
void	ft_signal_exit(int signal)
{
	char	*nbr;

	nbr = ft_itoa(signal);
	if (global_sig.child_proc_id != 0)
	{
		ft_putstr_fd("\b\b \b\b", STDERR);
		ft_putendl_fd(nbr, STDERR);
		global_sig.last_exit_stat = 131;
		global_sig.sigquit_received = 1;
	}
	else
		ft_putstr_fd("\b\b  \b\b", STDERR);
	ft_memdel(nbr);
}
