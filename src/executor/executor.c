/**
 * cmd_tab - Creates an array of strings (command table) from a linked list of tokens.
 * 
 * @start: A pointer to the starting token (t_cmd_token) for creating the command table.
 * 
 * This function iterates through a linked list of tokens starting from `start` and 
 * creates an array of strings (`tab`). It stops adding tokens when it encounters a 
 * token with a type greater than or equal to `TRUNC` (indicating a redirection or separator). 
 * The resulting array contains all token strings in the linked list up to this point, 
 * and the final element of the array is set to `NULL`.
 * 
 * Return:
 * - A pointer to an array of strings containing the command and its arguments.
 * - NULL if memory allocation fails or if the `start` token is `NULL`.
 */
// Pseudo Code for cmd_tab and exec_cmd
// Pseudo Code for cmd_tab
// Check if cmd_list is NULL:
// If it is NULL, return NULL.
// Initialize variables:
// Set token to the next token from start.
// Set i to 2.
// Count the number of tokens up to a redirection or separator:
// Iterate through the tokens while the token type is less than TRUNC.
// Increment i for each token found.
// Allocate memory for the command table (tab).
// If memory allocation fails, return NULL.
// Assign strings to the command table:
// Set tab[0] to cmd_lst->content.
// Iterate through the tokens and add each token string to tab[i] while the token type is less than TRUNC.
// Terminate the command table with a NULL pointer.
// Return the command table.

/**
 * exec_cmd - Executes a command token by expanding and running the command.
 * 
 * @shell_state: A pointer to the shell state structure (t_shell_state) containing environment variables and other state details.
 * @token: A pointer to the token (t_cmd_token) containing the command to be executed.
 * 
 * This function first converts the linked list of tokens into an array of strings using 
 * `cmd_tab()`. It then iterates through the array to expand any environment variables using 
 * the `expansions()` function. Depending on the command (`cmd`), it performs the following:
 * - If the command is `"exit"` and there is no active pipe, it exits the shell using `mini_exit()`.
 * - If the command is a built-in command, it executes it using `exec_builtin()`.
 * - Otherwise, it executes external binaries using `exec_bin()`.
 * 
 * After execution, the function frees the allocated memory for the command array and closes 
 * any active pipes. It also resets the relevant state in the `mini` structure.
 */

// Pseudo Code for exec_cmd
// Check if shell_state->is_foreground is 0:
// If it is 0, return without executing.
// Convert the tokens to a command table using cmd_tab().
// Expand environment variables:
// Iterate through the command table and call expansions() on each command/argument string.
// Check if the command is "exit":
// If "exit" is the command and there is no active pipe (has_pipe(token) == 0), call mini_exit().
// Check if the command is a built-in command:
// If it is a built-in command, call exec_builtin() and store the result in mini->ret.
// Otherwise, execute the command as an external binary using exec_bin().
// Free the command table (cmd) to avoid memory leaks.
// Close any active pipes by calling ft_close() on shell_state->pipe_input_fd and shell_state->pipe_output_fd.
// Reset pipe-related state in the mini structure:
// Set shell_state->pipe_input_fd and shell_state->pipe_output_fd to -1.
// Set shell_state->is_foreground to 1.