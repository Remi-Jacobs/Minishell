/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsamuel <dsamuel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 19:37:40 by dsamuel           #+#    #+#             */
/*   Updated: 2024/10/23 19:43:56 by dsamuel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * next_sep - Finds the next separator token in the command token list.
 * 
 * @token: A pointer to the current command token (t_cmd_token) being examined.
 * @skip: An integer indicating whether to skip the current token.
 * 
 * This function iterates through the list of command tokens starting from the given `token`. 
 * If `skip` is non-zero, it moves to the next token before checking. It then continues to 
 * iterate until it finds a token of type greater than or equal to `TRUNC`, which indicates 
 * a separator. It returns a pointer to the found separator token or `NULL` if none is found.
 * 
 * Return:
 * - A pointer to the next separator token (`TRUNC` or greater) in the list.
 * - NULL if no separator is found.
 */
t_cmd_token *next_sep(t_cmd_token *token, int skip)
{
    if (token && skip)
        token = token->next;
    while (token && token->type < TRUNC)
        token = token->next;
    return (token);
}

/**
 * prev_sep - Finds the previous separator token in the command token list.
 * 
 * @token: A pointer to the current command token (t_cmd_token) being examined.
 * @skip: An integer indicating whether to skip the current token.
 * 
 * This function iterates through the list of command tokens starting from the given `token`. 
 * If `skip` is non-zero, it moves to the previous token before checking. It then continues 
 * to iterate backward until it finds a token of type greater than or equal to `TRUNC`, 
 * which indicates a separator. It returns a pointer to the found separator token or `NULL` 
 * if none is found.
 * 
 * Return:
 * - A pointer to the previous separator token (`TRUNC` or greater) in the list.
 * - NULL if no separator is found.
 */
t_cmd_token *prev_sep(t_cmd_token *token, int skip)
{
    if (token && skip)
        token = token->prev;
    while (token && token->type < TRUNC)
        token = token->prev;
    return (token);
}

/**
 * next_run - Finds the next command token in the command token list.
 * 
 * @token: A pointer to the current command token (t_cmd_token) being examined.
 * @skip: An integer indicating whether to skip the current token.
 * 
 * This function iterates through the list of command tokens starting from the given `token`. 
 * If `skip` is non-zero, it moves to the next token before checking. It continues iterating 
 * until it finds a token of type `CMD`. If the command token is found and it has a preceding 
 * token of type less than `END`, it moves forward to find the next command token.
 * 
 * Return:
 * - A pointer to the next command token (`CMD`) in the list.
 * - NULL if no command token is found.
 */
t_cmd_token *next_run(t_cmd_token *token, int skip)
{
    if (token && skip)
        token = token->next;
    while (token && token->type != CMD)
    {
        token = token->next;
        if (token && token->type == CMD && token->prev == NULL)
            ;
        else if (token && token->type == CMD && token->prev->type < END)
            token = token->next;
    }
    return (token);
}