/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsamuel <dsamuel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 20:56:48 by dsamuel           #+#    #+#             */
/*   Updated: 2024/10/14 15:44:59 by dsamuel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

// int	main(int argc, char **argv, char **envp)
// {
// 	t_shell_state	shell_state;

// 	(void)argc;
// 	(void)argv;
// 	init_shell_state(&shell_state, envp);
// 	while (1)
// 	{
// 		shell_state.should_skip_exec = NOSKIP;
// 		shell_state.should_exit = 0;
// 		shell_state.is_foreground = 1;
// 		shell_state.is_parent_proc = 1;
// 		shell_state.return_code = 0;
// 		shell_state.last_exit_stat = 0;
// 		shell_state.proc_id = 0;
// 		shell_state.stdin_fd = dup(STDIN);
// 		shell_state.stdout_fd = dup(STDOUT);
// 		shell_state.input_fd = STDIN;
// 		shell_state.output_fd = STDOUT;
// 		shell_state.pipe_input_fd = STDIN;
// 		shell_state.pipe_output_fd = STDOUT;
// 		shell_state.cmd_list = NULL;
// 		shell_state.active_env = NULL;
// 		shell_state.secret_env = NULL;
// 		shell_state.active_env = parse_env(envp);
// 		shell_state.secret_env = NULL;
// 		shell_state.cmd_list = parse_input(&shell_state);
// 		if (shell_state.should_exit)
// 			break ;
// 		if (shell_state.should_skip_exec == NOSKIP)
// 			execute_commands(&shell_state);
// 		free_cmd_list(shell_state.cmd_list);
// 	}
// 	free_env_list(shell_state.active_env);
// 	free_env_list(shell_state.secret_env);
// 	return (shell_state.last_exit_stat);
// }

/**
 * @brief Handles redirections and command execution in the `minishell`.
 *
 * This function processes the parsed tokens to determine the type of redirection
 * or piping required and executes the command based on the redirection type.
 * It supports output truncation (`>`), output append (`>>`), input redirection (`<`),
 * and piping (`|`). It also manages the chaining of commands and recursive calls
 * for multi-command input.
 *
 * Workflow:
 * 1. Retrieve the previous and next separators (e.g., pipe, redirection).
 * 2. Determine the type of redirection based on the previous token:
 *    - If the previous token is `TRUNC`, perform output truncation.
 *    - If the previous token is `APPEND`, perform output append.
 *    - If the previous token is `INPUT`, perform input redirection.
 *    - If the previous token is `PIPE`, set up a pipe.
 * 3. If a next token exists and is not an `END` token, recursively call `redir_and_exec()`
 *    to handle the next command in the sequence.
 * 4. If the previous token is an `END` or `PIPE` or if no previous token exists,
 *    and no pipe is set up, execute the command using `exec_cmd()` if the `no_exec` flag is `0`.
 *
 * @param mini A pointer to the `t_mini` structure containing the shell state.
 * @param token A pointer to the current command token to be processed.
 */
void ft_redir_and_exec(t_shell_state *shell_state, t_cmd_token *cmd_token)
{
    t_cmd_token *prev;
    t_cmd_token *next;
    int pipe;

    // Get the previous and next separator tokens for the current token.
    prev = ft_prev_sep(cmd_token, NOSKIP);
    next = ft_next_sep(cmd_token, NOSKIP);
    pipe = 0;

    // Handle output truncation, output append, input redirection, or pipe setup.
    if (is_type(prev, TRUNC))
        ft_redir(shell_state, cmd_token, TRUNC);
    else if (is_type(prev, APPEND))
        ft_redir(shell_state, cmd_token, APPEND);
    else if (is_type(prev, INPUT))
        ft_input(shell_state, cmd_token);
    else if (is_type(prev, PIPE))
        pipe = ft_minipipe(shell_state);

    // If there is a next token and it's not an `END`, recursively process the next token.
    if (next && is_type(next, END) == 0 && pipe != 1)
        ft_redir_and_exec(shell_state, next->next);

    // If the command is standalone, follows an `END` or `PIPE`, and no pipe is set up,
    // execute the command if the `no_exec` flag is not set.
    if ((is_type(prev, END) || is_type(prev, PIPE) || !prev)
        && pipe != 1 && shell_state->should_skip_exec == 0)
        ft_exec_cmd(shell_state, cmd_token);
}

