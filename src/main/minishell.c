/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsamuel <dsamuel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 20:56:48 by dsamuel           #+#    #+#             */
/*   Updated: 2024/10/14 15:47:15 by dsamuel          ###   ########.fr       */
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

/**
 * @brief Main execution loop for processing and running commands in `minishell`.
 *
 * This function iteratively processes parsed command tokens, handles redirections,
 * and executes each command while maintaining the shell's state. It manages
 * input/output redirections, waits for child processes to finish, and updates
 * the return status of the shell based on the executed commands.
 *
 * Workflow:
 * 1. Determine the first executable token from the parsed input.
 * 2. Iterate over tokens while the shell is active (`mini->exit == 0`).
 *    - Set up shell state flags (`charge`, `parent`, `last`) for each command.
 *    - Call `redir_and_exec()` to handle redirections and execute the command.
 *    - Restore standard input/output and close any used file descriptors.
 *    - Wait for child processes using `waitpid()` and retrieve their status.
 *    - Update the shell's return status (`mini->ret`) based on the command's outcome.
 *    - If the current process is a child (`mini->parent == 0`), clean up and exit.
 *    - Move to the next executable token and reset the `no_exec` flag.
 * 3. Repeat until all tokens are processed or the shell is set to exit.
 *
 * @param mini A pointer to the `t_mini` structure, which holds the current
 *             shell state, including environment variables, file descriptors,
 *             and command tokens.
 */
void ft_minishell(t_shell_state *shell_state)
{
    t_cmd_token *cmd_token;
    int     status;
    

    // Get the first executable token and adjust if necessary.
    cmd_token = ft_next_exec(shell_state->cmd_list, NOSKIP);
    if(is_types(shell_state->cmd_list, "TAI"))
        cmd_token = shell_state->cmd_list->next;
    else
        cmd_token = cmd_token;
    // Main loop: Process tokens until `mini->exit` is set to non-zero.
    while (shell_state->should_exit == 0 && cmd_token)
    {
        // Set flags for command execution.
        shell_state->is_foreground = 1;   // Indicate that the current process is active.
        shell_state->is_parent_proc = 1;   // Set to 1 to denote that the process is the parent.
        shell_state->last_exit_stat = 1;     // Indicates that the command is the last executed.

        // Handle redirections and execute the command.
        ft_redir_and_exec(shell_state, cmd_token);

        // Restore standard I/O, close, and reset file descriptors.
        ft_reset_std(shell_state);
        ft_close_fds(shell_state);
        ft_reset_fds(shell_state);

        // Wait for child processes and retrieve their exit status.
        waitpid(-1, &status, 0);
        status = WEXITSTATUS(status);
        // Update return status based on the last executed command.
        if(shell_state->last_exit_stat == 0)
            shell_state->return_code = status;
        else
            shell_state->return_code = shell_state->last_exit_stat;
        // If this is a child process, clean up and exit.
        if (shell_state->is_parent_proc == 0)
        {
            ft_free_token(cmd_token);
            exit(shell_state->return_code);
        }
        

        // Reset execution flag and proceed to the next token.
        shell_state->should_skip_exec = 0;
        cmd_token = ft_next_exec(cmd_token->next, NOSKIP);
    }
}


/**
 * @file minishell.c
 * @brief Main entry point for the custom minishell implementation.
 *
 * This function sets up the initial shell state and manages the interactive 
 * loop of the shell, allowing the user to input commands, parse them, 
 * and execute them. It also handles environment initialization, signal handling, 
 * and cleanup upon exit.
 *
 * The shell uses the following workflow:
 * 1. Initializes file descriptors for input and output.
 * 2. Sets up environment variables and increments the shell level.
 * 3. Enters a loop that continues until the `exit` command is issued.
 *    - In each iteration, it resets signal handlers and parses the user's input.
 *    - If the parsed input is valid, it calls `minishell` to execute the commands.
 *    - Frees any tokens after command execution to prevent memory leaks.
 * 4. When the loop ends, it frees all allocated environment variables and 
 *    returns the last command's exit status.
 *
 * @param ac Argument count (unused).
 * @param av Argument vector (unused).
 * @param env Environment variables passed to the shell at startup.
 * @return int Returns the last executed command's exit status.
 */

int	main(int argc, char **argv, char **envp)
{
    t_shell_state mini_shell;

    (void)argc;
	(void)argv;
    // Duplicate the standard input and output file descriptors to preserve them
    mini_shell.stdin_fd = dup(STDIN);
	mini_shell.stdout_fd = dup(STDOUT);
    // Initialize shell control variables
    mini_shell.should_exit = 0;  // Flag to control shell exit state
    mini_shell.return_code = 0;     // Store the return status of the last executed command
    mini_shell.should_skip_exec = 0;// Flag to indicate if commands should be executed

    // Reset standard file descriptors to their initial state
    ft_reset_fds(&mini_shell);
    // Initialize environment variables and increment the shell level (SHLVL)
    ft_env_init(&mini_shell, envp);
	ft_secret_env_init(&mini_shell, envp);
    ft_increment_shell_level(mini_shell.active_env);

    // Main interactive loop - continues until `mini.exit` is set to non-zero
    while (mini_shell.should_exit == 0)
    {
        // Set up signal handling (e.g., for Ctrl+C or Ctrl+\)
        ft_sig_init();
        // Parse user input into command tokens
        ft_parse_input(&mini_shell);
        // Check if parsing succeeded and the input line is valid
        if (mini_shell.cmd_list != NULL && ft_check_line(&mini_shell, mini_shell.cmd_list))
		{
			// Execute the parsed commands
			ft_mini_shell(&mini_shell);
		}
        // Free the memory allocated for tokens after processing
        ft_free_token(&mini_shell.cmd_list);
    }

    // Free all allocated environment variables before exiting
    ft_free_env(mini_shell.active_env);
    ft_free_env(mini_shell.secret_env);

    // Return the exit status of the last executed command
    return (mini_shell.return_code);
}