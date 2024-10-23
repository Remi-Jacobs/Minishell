/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   type.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsamuel <dsamuel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 19:44:42 by dsamuel           #+#    #+#             */
/*   Updated: 2024/10/23 19:51:51 by dsamuel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * is_type - Checks if the given token is of a specific type.
 * 
 * @token: A pointer to the command token (t_cmd_token) to be checked.
 * @type: The integer value representing the type to check.
 * 
 * This function verifies if the provided `token` is of the specified `type`. If the token 
 * exists and its type matches the given type, it returns 1.
 * 
 * Return:
 * - 1 if the token exists and its type matches the specified type.
 * - 0 otherwise.
 */
int is_type(t_cmd_token *token, int type)
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
int is_types(t_cmd_token *token, char *types)
{
    if (ft_strchr(types, ' ') && is_type(token, EMPTY))
        return (1);
    else if (ft_strchr(types, 'X') && is_type(token, CMD))
        return (1);
    else if (ft_strchr(types, 'x') && is_type(token, ARG))
        return (1);
    else if (ft_strchr(types, 'T') && is_type(token, TRUNC))
        return (1);
    else if (ft_strchr(types, 'A') && is_type(token, APPEND))
        return (1);
    else if (ft_strchr(types, 'I') && is_type(token, INPUT))
        return (1);
    else if (ft_strchr(types, 'P') && is_type(token, PIPE))
        return (1);
    else if (ft_strchr(types, 'E') && is_type(token, END))
        return (1);
    return (0);
}

/**
 * has_type - Checks if a linked list of tokens contains a specific type.
 * 
 * @token: A pointer to the head of the command token list (t_cmd_token).
 * @type: The integer value representing the type to search for.
 * 
 * This function iterates through a linked list of command tokens and checks if any of the 
 * tokens are of the specified `type`. If a token with the matching type is found, it returns 1.
 * 
 * Return:
 * - 1 if a token with the specified type is found in the list.
 * - 0 otherwise.
 */
int has_type(t_cmd_token *token, int type)
{
    while (token)
    {
        if (is_type(token, type))
            return (1);
        token = token->next;
    }
    return (0);
}

/**
 * has_pipe - Checks if a linked list of tokens contains a pipe token before an end token.
 * 
 * @token: A pointer to the head of the command token list (t_cmd_token).
 * 
 * This function iterates through the linked list of command tokens and checks if there is 
 * a token of type `PIPE` before encountering a token of type `END`. If a pipe token is found 
 * before an end token, it returns 1.
 * 
 * Return:
 * - 1 if a pipe token is found before an end token.
 * - 0 otherwise.
 */
int has_pipe(t_cmd_token *token)
{
    while (token && is_type(token, END) == 0)
    {
        if (is_type(token, PIPE))
            return (1);
        token = token->next;
    }
    return (0);
}

/**
 * next_type - Finds the next token of a specific type in the command token list.
 * 
 * @token: A pointer to the current command token (t_cmd_token).
 * @type: The integer value representing the type to search for.
 * @skip: An integer indicating whether to skip the current token.
 * 
 * This function iterates through the list of command tokens starting from the given `token`. 
 * If `skip` is non-zero, it moves to the next token before starting the search. It continues 
 * to iterate until it finds a token of the specified `type` or reaches the end of the list.
 * 
 * Return:
 * - A pointer to the next token of the specified type.
 * - NULL if no token of the specified type is found.
 */
t_cmd_token *next_type(t_cmd_token *token, int type, int skip)
{
    if (token && skip)
        token = token->next;
    while (token && token->type != type)
        token = token->next;
    return (token);
}