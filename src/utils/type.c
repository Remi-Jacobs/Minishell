/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   type.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsamuel <dsamuel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 19:44:42 by dsamuel           #+#    #+#             */
/*   Updated: 2024/11/07 15:46:34 by dsamuel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_is_type(t_cmd_token *token, int type)
{
	if (token && token->type == type)
		return (1);
	else
		return (0);
}

/**
 * is_types - Checks if the token is of any type listed in the given string.
 * 
 * @token: A pointer to the command token (t_cmd_token) to be checked.
 * @types: A string containing characters that represent different token types.
 * 
 * This function checks if the provided `token` matches any of the types specified in the 
 * `types` string. Each character in the string corresponds to a particular type:
 * - `' '` for EMPTY
 * - `'X'` for CMD
 * - `'x'` for ARG
 * - `'T'` for TRUNC
 * - `'A'` for APPEND
 * - `'I'` for INPUT
 * - `'P'` for PIPE
 * - `'E'` for END
 * 
 * Return:
 * - 1 if the token matches any of the types listed in the string.
 * - 0 otherwise.
 */
int	ft_is_types(t_cmd_token *token, char *types)
{
	if (ft_strchr(types, ' ') && ft_is_type(token, EMPTY))
		return (1);
	else if (ft_strchr(types, 'X') && ft_is_type(token, CMD))
		return (1);
	else if (ft_strchr(types, 'x') && ft_is_type(token, ARG))
		return (1);
	else if (ft_strchr(types, 'T') && ft_is_type(token, TRUNC))
		return (1);
	else if (ft_strchr(types, 'A') && ft_is_type(token, APPEND))
		return (1);
	else if (ft_strchr(types, 'I') && ft_is_type(token, INPUT))
		return (1);
	else if (ft_strchr(types, 'P') && ft_is_type(token, PIPE))
		return (1);
	else if (ft_strchr(types, 'E') && ft_is_type(token, END))
		return (1);
	return (0);
}

int	ft_has_type(t_cmd_token *token, int type)
{
	while (token)
	{
		if (ft_is_type(token, type))
			return (1);
		token = token->next;
	}
	return (0);
}

int	ft_has_pipe(t_cmd_token *token)
{
	while (token && ft_is_type(token, END) == 0)
	{
		if (ft_is_type(token, PIPE))
			return (1);
		token = token->next;
	}
	return (0);
}

t_cmd_token	*ft_next_type(t_cmd_token *token, int type, int skip)
{
	if (token && skip)
		token = token->next;
	while (token && token->type != type)
		token = token->next;
	return (token);
}
