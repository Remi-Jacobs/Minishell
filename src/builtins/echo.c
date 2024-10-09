/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ojacobs <ojacobs@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 14:31:30 by ojacobs           #+#    #+#             */
/*   Updated: 2024/10/09 20:12:28 by ojacobs          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <string.h>

void	cmd_echo(char **args)
{
	int	newline;
	int	i;

	newline = 1;
	i = 1;
	// replace with ft_strcmp in libft
	if (args[1] && strcmp(args[1], "-n") == 0)
	{
		newline = 0;
		i = 2;
	}
	while (args[i])
	{
		printf("%s", args[i]);
		if (args[i + 1])
			printf(" ");
		i++;
	}
	if (newline)
		printf("\n");
}


// int	main (int ac, char **argv)
// {
// 	cmd_echo(argv);
// 	return (0);
// }
