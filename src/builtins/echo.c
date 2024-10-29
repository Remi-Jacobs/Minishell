/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsamuel <dsamuel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 14:31:30 by ojacobs           #+#    #+#             */
/*   Updated: 2024/10/29 16:54:48 by dsamuel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*here the implementation of the echo I think this works a bit better
**
 * @brief Built-in command that echoes the arguments to the standard output.
 *
 * The `echo` command writes the arguments passed to it to the standard output.
 * If the `-n` option is provided as the first argument, the trailing newline
 * character is omitted. The command is a built-in function and does not require
 * a separate process to execute.
 *
 * @param args An array of strings containing the command arguments.
 */

static int	nb_args(char **args)
{
	int	size;

	size = 0;
	while (args[size])
		size++;
	return (size);
}


int				ft_echo(char **args)
{
	int		i;
	int		n_option;

	i = 1;
	n_option = 0;
	if (nb_args(args) > 1)
	{
		while (args[i] && ft_strcmp(args[i], "-n") == 0)
		{
			n_option = 1;
			i++;
		}
		while (args[i])
		{
			ft_putstr_fd(args[i], 1);
			if (args[i + 1] && args[i][0] != '\0')
				write(1, " ", 1);
			i++;
		}
	}
	if (n_option == 0)
		write(1, "\n", 1);
	return (SUCCESS);
}

// int	ft_echo(char **args)
// {
// 	int		i;
// 	int		n_flag;

// 	i = 1;
// 	n_flag = 0;
// 	while (nb_args(args) > 1 && ft_strcmp(args[i], "-n") == 0)
// 	{
// 		n_flag = 1;
// 		i++;
// 	}
// 	while (args[i])
// 	{
// 		ft_putstr_fd(args[i], STDOUT);
// 		if (args[i + 1] && args[i][0] != '\0')
// 			write(1, " ", 1);
// 		i++;
// 	}
// 	if (n_flag == 0)
// 		write(1, "\n", 1);
// 	return (SUCCESS);
// }
