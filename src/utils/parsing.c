/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsamuel <dsamuel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 19:29:22 by dsamuel           #+#    #+#             */
/*   Updated: 2024/10/23 19:37:26 by dsamuel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * is_sep - Checks if a character in the line is a separator and not escaped.
 * 
 * @line: A pointer to the line of text being parsed.
 * @i: The current index of the character to check in the line.
 * 
 * This function checks if the character at position `i` in the line is one of the separator 
 * characters (`<`, `>`, `|`, `;`) and not escaped by a preceding backslash (`\`). It also checks 
 * if the character is not inside a pair of quotes using the `quotes()` function.
 * 
 * Return:
 * - 1 if the character is a separator and not escaped.
 * - 0 otherwise.
 */
int is_sep(char *line, int i)
{
    if (i > 0 && line[i - 1] == '\\' && ft_strchr("<>|;", line[i]))
        return (0);
    else if (ft_strchr("<>|;", line[i]) && quotes(line, i) == 0)
        return (1);
    else
        return (0);
}

/**
 * ignore_sep - Checks if a separator character is escaped by a backslash.
 * 
 * @line: A pointer to the line of text being parsed.
 * @i: The current index of the character to check in the line.
 * 
 * This function checks if the separator characters (`;`, `|`, `>`, `>>`) at the current 
 * index `i` in the line are escaped by a preceding backslash (`\`). If a valid escape 
 * sequence is detected, it returns 1, indicating that the separator should be ignored.
 * 
 * Return:
 * - 1 if the separator is escaped by a backslash.
 * - 0 otherwise.
 */
int ignore_sep(char *line, int i)
{
    if (line[i] && line[i] == '\\' && line[i + 1] && line[i + 1] == ';')
        return (1);
    else if (line[i] && line[i] == '\\' && line[i + 1] && line[i + 1] == '|')
        return (1);
    else if (line[i] && line[i] == '\\' && line[i + 1] && line[i + 1] == '>')
        return (1);
    else if (line[i] && line[i] == '\\' && line[i + 1] && line[i + 1] == '>'
             && line[i + 2] && line[i + 2] == '>')
        return (1);
    return (0);
}

/**
 * quotes - Determines if the current index is within a pair of quotes.
 * 
 * @line: A pointer to the line of text being parsed.
 * @index: The current index to check in the line.
 * 
 * This function iterates through the line until it reaches the given `index`. It keeps 
 * track of whether it is inside double quotes (`"`) or single quotes (`'`). If a backslash 
 * precedes a quote character, it skips the quote. It returns a value indicating whether 
 * the index is inside a pair of quotes.
 * 
 * Return:
 * - 0 if not within quotes.
 * - 1 if within double quotes.
 * - 2 if within single quotes.
 */
int quotes(char *line, int index)
{
    int i;
    int open;

    i = 0;
    open = 0;
    while (line[i] && i != index)
    {
        if (i > 0 && line[i - 1] == '\\')
            ;
        else if (open == 0 && line[i] == '\"')
            open = 1;
        else if (open == 0 && line[i] == '\'')
            open = 2;
        else if (open == 1 && line[i] == '\"')
            open = 0;
        else if (open == 2 && line[i] == '\'')
            open = 0;
        i++;
    }
    return (open);
}

/**
 * is_last_valid_arg - Checks if the given token is a valid last argument.
 * 
 * @token: A pointer to the current token (t_cmd_token) being checked.
 * 
 * This function checks if the given `token` is of type `CMD` or `ARG`. It also verifies 
 * if there are no previous separators or if the previous separator is of type `END` or 
 * `PIPE`. If these conditions are met, the token is considered a valid last argument.
 * 
 * Return:
 * - 1 if the token is a valid last argument.
 * - 0 otherwise.
 */
int is_last_valid_arg(t_cmd_token *token)
{
    t_cmd_token *prev;

    if (!token || is_type(token, CMD) || is_type(token, ARG))
    {
        prev = prev_sep(token, NOSKIP);
        if (!prev || is_type(prev, END) || is_type(prev, PIPE))
            return (1);
        return (0);
    }
    else
        return (0);
}

/**
 * check_line - Checks the syntax of a line of command tokens for errors.
 * 
 * @shell_state: A pointer to the shell state structure (t_shell_state) containing shell data.
 * @token: A pointer to the first command token (t_cmd_token) in the list.
 * 
 * This function iterates through a list of command tokens and checks for syntax errors. 
 * It identifies errors such as invalid placement of separators and unexpected tokens like 
 * pipes and ends. If an error is detected, it prints an appropriate error message to 
 * standard error and sets the return code to 258.
 * 
 * Return:
 * - 1 if the syntax check passes without errors.
 * - 0 if a syntax error is detected.
 */
int check_line(t_shell_state *shell_state, t_cmd_token *token)
{
    while (token)
    {
        if (is_types(token, "TAI") && (!token->next || is_types(token->next, "TAIPE")))
        {
            ft_putstr_fd("bash: syntax error near unexpected token `", STDERR);
            if (token->next)
                ft_putstr_fd(token->next->content, STDERR);
            else
                ft_putstr_fd("newline", STDERR);
            ft_putendl_fd("'", STDERR);
            shell_state->return_code = 258;
            return (0);
        }
        if (is_types(token, "PE") && (!token->prev || !token->next || is_types(token->prev, "TAIPE")))
        {
            ft_putstr_fd("bash: syntax error near unexpected token `", STDERR);
            ft_putstr_fd(token->content, STDERR);
            ft_putendl_fd("'", STDERR);
            shell_state->return_code = 258;
            return (0);
        }
        token = token->next;
    }
    return (1);
}
