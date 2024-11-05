/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_line.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsamuel <dsamuel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 21:15:14 by dsamuel           #+#    #+#             */
/*   Updated: 2024/11/05 14:09:18 by dsamuel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * space_alloc - Allocates memory for a new line with extra spaces around separators.
 * 
 * @line: A pointer to the original line of text to be processed.
 * 
 * This function calculates the number of separators in the given `line` and allocates 
 * memory for a new string that will include additional spaces around the separators. 
 * It iterates through the original line to count the separators using `is_sep()`, then 
 * allocates memory accordingly to account for the extra spaces.
 * 
 * Return:
 * - A pointer to the newly allocated string with extra space allocation.
 * - NULL if memory allocation fails.
 */
char *ft_space_alloc(char *line)
{
    char *new;
    int count;
    int i;

    count = 0;
    i = 0;
    while (line[i])
    {
        if (ft_is_sep(line, i))
            count++;
        i++;
    }
    if (!(new = malloc(sizeof(char) * (i + 2 * count + 1))))
        return (NULL);
    return (new);
}

/**
 * space_line - Creates a new line with spaces added around separators.
 * 
 * @line: A pointer to the original line of text to be processed.
 * 
 * This function allocates memory for a new line using `space_alloc()`, then iterates 
 * through the original `line`. It checks for quotes and separators, and if a separator 
 * is found outside of quotes, it inserts spaces around it in the new line. Additionally, 
 * it handles cases where the `$` character is not escaped and modifies its representation.
 * 
 * Return:
 * - A pointer to the newly allocated and processed line with spaces around separators.
 * - NULL if memory allocation fails.
 */
char *ft_space_line(char *line)
{
    char *new;
    int i;
    int j;

    i = 0;
    j = 0;
    new = ft_space_alloc(line);
    while (new && line[i])
    {
        if (ft_quotes(line, i) != 2 && line[i] == '$' && i && line[i - 1] != '\\')
            new[j++] = (char)(-line[i++]);
        else if (ft_quotes(line, i) == 0 && ft_is_sep(line, i))
        {
            new[j++] = ' ';
            new[j++] = line[i++];
            if (ft_quotes(line, i) == 0 && line[i] == '>')
                new[j++] = line[i++];
            new[j++] = ' ';
        }
        else
            new[j++] = line[i++];
    }
    new[j] = '\0';
    ft_memdel(line);
    return (new);
}

/**
 * quote_check - Checks for unclosed quotes in the given line.
 * 
 * @mini: A pointer to the main shell state structure (t_mini).
 * @line: A double pointer to the line of text to be processed.
 * 
 * This function checks for unclosed quotes using the `quotes()` function. If open quotes 
 * are detected, it prints an error message, frees the line, and sets relevant flags and 
 * return codes in the `mini` structure. It prevents further processing of an invalid line.
 * 
 * Return:
 * - 1 if there is an open quotes syntax error.
 * - 0 if no syntax error is detected.
 */
int ft_quote_check(t_shell_state *shell_state, char **line)
{
    if (ft_quotes(*line, 2147483647))
    {
        ft_putendl_fd("syntax error with open quotes", STDERR);
        ft_memdel(*line);
        shell_state->return_code = 2;
        shell_state->active_env = NULL;
        return (1);
    }
    return (0);
}

/**
 * parse - Parses the input line and tokenizes it for further processing.
 * 
 * @mini: A pointer to the main shell state structure (t_mini).
 * 
 * This function handles the parsing of input from the user. It sets up signal handlers 
 * and displays the shell prompt. The function reads a line of input, checks for unclosed 
 * quotes using `quote_check()`, and if the line is valid, it processes separators using 
 * `space_line()`. It then tokenizes the line using `get_tokens()`, and adjusts arguments 
 * with `squish_args()`. Finally, it iterates through the tokens to identify and process 
 * arguments using `type_arg()`.
 * 
 * Return:
 * - This function does not return a value.
 */

void ft_parse_input(t_shell_state *shell_state)
{
    char *line;
    t_cmd_token *token;

    // Set signal handlers
    signal(SIGINT, &ft_sig_integer);
    signal(SIGQUIT, &ft_sig_exit);

    // Print the prompt based on the return code
    if (shell_state->return_code)
        ft_putstr_fd("🤬 ", STDERR);
    else
        ft_putstr_fd("😎 ", STDERR);

    // Print the shell prompt
    // ft_putstr_fd("\033[0;36m\033[1mminishell ▸ \033[0m", STDERR); chng 1
    line = readline("\033[0;36m\033[1mminishell ▸ \033[0m");
    // Get the next line of input and check for the exit condition
    //  change 2
    if (!line)
    {
        shell_state->should_exit = 1;
        ft_putendl_fd("exit", STDERR);
        return ;
    }
    if (*line)
        add_history(line);
    // Update the return code if an interrupt signal was received
    if (global_sig.sigint_received == 1)
        shell_state->return_code = global_sig.last_exit_stat;

    // Check for unclosed quotes
    if (ft_quote_check(shell_state, &line))
    {
        ft_memdel(line);
        return ;
    }
    // Process the line to add spaces around separators
    line = ft_space_line(line);

    // Handle special case with '$' at the start of the line
    if (line && line[0] == '$')
        line[0] = (char)(-line[0]);

    // Tokenize the line and update the shell state
    shell_state->cmd_list = ft_get_tokens(line);
    ft_memdel(line);

    // Adjust and squish the arguments in the token list
    ft_squish_args(shell_state);

    // Iterate through the tokens and handle arguments
    token = shell_state->cmd_list;
    while (token)
    {
        if (ft_is_type(token, ARG))
            ft_type_arg(token, 0);
        token = token->next;
    }
}
