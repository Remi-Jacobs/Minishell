/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsamuel <dsamuel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 20:00:14 by dsamuel           #+#    #+#             */
/*   Updated: 2024/11/22 17:03:21 by dsamuel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_sig_integer(int signal)
{
	(void)signal;
	if (g_global_sig.child_proc_id == 0)
	{
		ft_putstr_fd("\b\b", STDERR);
		ft_putstr_fd("\n", STDERR);
		// ft_putstr_fd("\033[0;36m\033[1m😎 minishell ▸ \033[0m", STDERR);
		g_global_sig.last_exit_stat = 1;
	}
	else
	{
		ft_putstr_fd("\n", STDERR);
		g_global_sig.last_exit_stat = 130;
	}
	g_global_sig.sigint_received = 1;
}

void	ft_sig_exit(int signal)
{
	char	*nbr;

	nbr = ft_itoa(signal);
	if (g_global_sig.child_proc_id != 0)
	{
		ft_putstr_fd("Quit:", STDERR);
		ft_putendl_fd(nbr, STDERR);
		g_global_sig.last_exit_stat = 131;
		g_global_sig.sigquit_received = 1;
	}
	else
		ft_putstr_fd("\b\b  \b\b", STDERR);
	ft_memdel(nbr);
}

void	ft_sig_init(void)
{
	g_global_sig.sigint_received = 0;
	g_global_sig.sigquit_received = 0;
	g_global_sig.child_proc_id = 0;
	g_global_sig.last_exit_stat = 0;
}
