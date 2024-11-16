/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsamuel <dsamuel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 20:56:48 by dsamuel           #+#    #+#             */
/*   Updated: 2024/11/15 20:11:44 by dsamuel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

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

t_sig_handler	global_sig;


void ft_redir_and_exec(t_shell_state *shell_state, t_cmd_token *cmd_token)
{
    t_cmd_token *prev;
    t_cmd_token *next;
    int pipe;
    // shell_state->here_doc_triggered += 2;  // Flag to track if here-doc has been triggered

    prev = ft_prev_sep(cmd_token, NOSKIP);
    next = ft_next_sep(cmd_token, NOSKIP);
    pipe = 0;

    // Ensure here-doc is triggered only once
    if (ft_is_type(prev, HERE_DOC) && shell_state->here_doc_triggered == 0 )
    {
		shell_state->here_doc_triggered = 1;  // Mark as triggered
        ft_here_doc(shell_state, cmd_token);  // Trigger here-doc
        // shell_state->here_doc_triggered += 1;  // Mark as triggered
    }
    else if (ft_is_type(prev, TRUNC))
        ft_redir(shell_state, cmd_token, TRUNC);
    else if (ft_is_type(prev, APPEND))
        ft_redir(shell_state, cmd_token, APPEND);
    else if (ft_is_type(prev, INPUT))
        ft_input(shell_state, cmd_token);
    else if (ft_is_type(prev, PIPE))
        pipe = ft_mini_pipe(shell_state);

    if (next && ft_is_type(next, END) == 0 && pipe != 1)
        ft_redir_and_exec(shell_state, next->next);

    if ((ft_is_type(prev, END) || ft_is_type(prev, PIPE) || !prev) && pipe != 1 && shell_state->should_skip_exec == 0)
    {
        // Reset here-doc flag after execution to allow future commands to process normally
        ft_exec_cmd(shell_state, cmd_token);
    }
}



// void	ft_redir_and_exec(t_shell_state *shell_state, t_cmd_token *cmd_token)
// {
// 	t_cmd_token	*prev;
// 	t_cmd_token	*next;
// 	int			pipe;

// 	prev = ft_prev_sep(cmd_token, NOSKIP);
// 	next = ft_next_sep(cmd_token, NOSKIP);
// 	pipe = 0;
// 	if (ft_is_type(prev, TRUNC))
// 		ft_redir(shell_state, cmd_token, TRUNC);
// 	else if (ft_is_type(prev, APPEND))
// 		ft_redir(shell_state, cmd_token, APPEND);
// 	else if (ft_is_type(prev, HERE_DOC))
// 	{
// 		write(1,"Here doc at redirect\n",22);
// 		ft_here_doc(shell_state, cmd_token);
// 	}
// 	else if (ft_is_type(prev, INPUT))
// 		ft_input(shell_state, cmd_token);
// 	else if (ft_is_type(prev, PIPE))
// 		pipe = ft_mini_pipe(shell_state);
// 	if (next && ft_is_type(next, END) == 0 && pipe != 1)
// 		ft_redir_and_exec(shell_state, next->next);
// 	if ((ft_is_type(prev, END) || ft_is_type(prev, PIPE) || !prev)
// 		&& pipe != 1 && shell_state->should_skip_exec == 0)
// 		ft_exec_cmd(shell_state, cmd_token);
// }

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
void	ft_mini_shell(t_shell_state *shell_state)
{
	t_cmd_token	*cmd_token;
	int			status;

	cmd_token = ft_next_run(shell_state->cmd_list, NOSKIP);
	if (ft_is_types(shell_state->cmd_list, "TAI"))
		cmd_token = shell_state->cmd_list->next;
	else
		cmd_token = cmd_token;
	while (shell_state->should_exit == 0 && cmd_token)
	{
		shell_state->is_foreground = 1;
		shell_state->is_parent_proc = 1;
		shell_state->last_exit_stat = 1;
		ft_redir_and_exec(shell_state, cmd_token);
		ft_reset_std(shell_state);
		ft_close_fds(shell_state);
		ft_reset_fds(shell_state);
		waitpid(-1, &status, 0);
		status = WEXITSTATUS(status);
		if (shell_state->last_exit_stat == 0)
			shell_state->return_code = status;
		else
			shell_state->return_code = shell_state->return_code;
		if (shell_state->is_parent_proc == 0)
		{
			ft_free_token(shell_state->cmd_list);
			exit(shell_state->return_code);
		}
		shell_state->should_skip_exec = 0;
		cmd_token = ft_next_run(cmd_token, SKIP);
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
	t_shell_state	shell_state;

	(void)argc;
	(void)argv;
	shell_state.stdin_fd = dup(STDIN);
	shell_state.stdout_fd = dup(STDOUT);
	shell_state.should_exit = 0;
	shell_state.return_code = 0;
	shell_state.should_skip_exec = 0;
	shell_state.history_count = 0;
	shell_state.here_doc_triggered = 0;
	ft_reset_fds(&shell_state);
	ft_env_init(&shell_state, envp);
	ft_secret_env_init(&shell_state, envp);
	ft_increment_shell_level(shell_state.active_env);
	while (shell_state.should_exit == 0)
	{
		ft_sig_init();
		ft_parse_input(&shell_state);
		if (shell_state.cmd_list != NULL
			&& ft_check_line(&shell_state, shell_state.cmd_list))
			ft_mini_shell(&shell_state);
		ft_free_token(shell_state.cmd_list);
	}
	ft_free_history(shell_state);
	ft_free_env(shell_state.active_env);
	ft_free_env(shell_state.secret_env);
	ft_memdel(shell_state.tilde);
	return (shell_state.return_code);
}
