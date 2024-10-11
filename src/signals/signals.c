/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsamuel <dsamuel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 20:00:14 by dsamuel           #+#    #+#             */
/*   Updated: 2024/10/11 20:54:38 by dsamuel          ###   ########.fr       */
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

void*ft_memdel(void *ptr)
{
	if (ptr)
	{
		free(ptr);
		ptr = NULL;
	}
	return (NULL);
}

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
