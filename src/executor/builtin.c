/**
 * is_builtin - Checks if a command is a built-in shell command.
 * 
 * @command: A pointer to a string containing the command to check.
 * 
 * This function compares the given command string with known built-in shell commands. 
 * It checks for the following built-in commands: "echo", "cd", "pwd", "env", "export", 
 * and "unset". If the command matches one of these, it returns 1, indicating that it is 
 * a built-in command.
 * 
 * Return:
 * - 1 if the command is a built-in command.
 * - 0 otherwise.
 */

// Pseudo Code for is_builtin and exec_builtin
// Pseudo Code for is_builtin
// Compare the command string with known built-in commands:
// If the command is "echo", return 1.
// If the command is "cd", return 1.
// If the command is "pwd", return 1.
// If the command is "env", return 1.
// If the command is "export", return 1.
// If the command is "unset", return 1.
// If no match is found, return 0.

/**
 * exec_builtin - Executes a built-in shell command.
 * 
 * @args: A double pointer to an array of strings representing the command and its arguments.
 * @shell_state: A pointer to the shell state structure (t_mini) containing environment variables.
 * 
 * This function checks the first argument (`args[0]`) to determine which built-in command 
 * to execute. Depending on the command, it calls the appropriate function with the given 
 * arguments and environment variables. The commands handled include "echo", "cd", "pwd", 
 * "env", "export", and "unset". It returns the result of the executed built-in command.
 * 
 * Return:
 * - The result of executing the built-in command.
 */
// Pseudo Code for exec_builtin
// Initialize a result variable to 0.
// Check the first argument (args[0]) to determine which built-in command to execute:
// If "echo":
// Call ft_echo() with the arguments and assign the return value to result.
// If "cd":
// Call ft_cd() with the arguments and environment variables, and assign the return value to result.
// If "pwd":
// Call ft_pwd() and assign the return value to result.
// If "env":
// Call ft_env() with the environment variables.
// If "export":
// Call ft_export() with the arguments, environment variables, and secret environment variables.
// If "unset":
// Call ft_unset() with the arguments and shell state structure.
// Return the result of executing the built-in command.