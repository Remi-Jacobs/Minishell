/**
 * error_message - Prints an appropriate error message based on the provided path.
 * 
 * @path: A pointer to a string containing the path of the file or command.
 * 
 * This function checks if the specified `path` is a command, a directory, or a file 
 * with permission issues. It uses `open` and `opendir` to determine the nature of the 
 * path and prints a suitable error message to standard error (`STDERR`).
 * 
 * Return:
 * - `UNKNOWN_COMMAND` if the path is not found or is an invalid command.
 * - `IS_DIRECTORY` if the path is a directory.
 */


// Open the path as a file and attempt to open it as a directory.
// Print an error message based on the following checks:
// If the path does not contain a /, print "command not found".
// If the file could not be opened and it is not a directory, print "No such file or directory".
// If the file is a directory, print "is a directory".
// If the file exists but permissions are denied, print "Permission denied".
// Determine the return value:
// If the path is not a command, return UNKNOWN_COMMAND.
// If the path is a directory, return IS_DIRECTORY.
// Close the directory and file if they were opened.


/**
 * magic_box - Executes a command using `execve` after forking a child process.
 * 
 * @path: A pointer to a string containing the path of the executable command.
 * @args: A double pointer to an array of strings representing command-line arguments.
 * @env: A pointer to the environment variable list (`t_env_variable`).
 * @shell_state: A pointer to the shell state structure (`t_shell_state`).
 * 
 * This function forks a child process to execute a command using `execve`. It constructs 
 * an environment array from the environment variable list, checks if the `path` contains 
 * a directory separator, and attempts to execute the command. If execution fails, it calls 
 * `error_message()` to print an appropriate error message and returns the error code.
 * 
 * Return:
 * - The exit status of the child process.
 * - A custom error code based on the result of `execve` or signal termination.
 */

// Pseudo Code for magic_box
// Fork the current process.
// In the child process:
// Convert the environment variables to an array of strings.
// If the path contains a /, attempt to execute the command using execve().
// If execution fails, call error_message() to print an error message.
// Free allocated memory and exit with the error code.
// In the parent process:
// Wait for the child process to finish.
// Check if a signal (SIGINT or SIGQUIT) was received:
// If so, return the corresponding exit status.
// Return the exit status of the child process.

/**
 * path_join - Joins two path components with a forward slash (`/`).
 * 
 * @s1: A pointer to the first path component.
 * @s2: A pointer to the second path component.
 * 
 * This function concatenates two path components (`s1` and `s2`) with a forward slash (`/`) 
 * between them. It returns the newly created path string.
 * 
 * Return:
 * - A pointer to a newly allocated string containing the joined path.
 */

// Pseudo Code for path_join
// Concatenate the first path component (s1) with a /.
// Concatenate the resulting string with the second path component (s2).
// Free the temporary string used for the first concatenation.
// Return the final joined path.

/**
 * check_dir - Searches for a command in a specified directory.
 * 
 * @bin: A pointer to a string containing the directory path.
 * @command: A pointer to a string containing the command name.
 * 
 * This function opens the specified directory (`bin`) and searches for an entry that 
 * matches the given command name (`command`). If the command is found, it returns the 
 * full path to the command by joining the directory and command name.
 * 
 * Return:
 * - A pointer to the full path of the command if found.
 * - NULL if the command is not found or if the directory cannot be opened.
 */

// Pseudo Code for check_dir
// Open the specified directory (bin).
// If the directory cannot be opened, return NULL.
// Iterate through the directory entries:
// If an entry matches the command name (command), create the full path by joining the directory and command name.
// Return the full path if the command was found, otherwise return NULL.

/**
 * exec_bin - Executes a binary file found in the system's PATH.
 * 
 * @args: A double pointer to an array of strings representing command-line arguments.
 * @env: A pointer to the environment variable list (`t_env_variable`).
 * @shell_state: A pointer to the shell state structure (`t_shell_state`).
 * 
 * This function searches for the specified binary command in the directories listed 
 * in the `PATH` environment variable. If the command is found, it calls `magic_box()` 
 * to execute it. If the command is not found, it calls `magic_box()` with the original 
 * command name to print an appropriate error message.
 * 
 * Return:
 * - The exit status or error code of the executed command.
 */

// Pseudo Code for exec_bin
// Search for the PATH environment variable.
// If PATH is not found, call magic_box() with the original command name.
// Split the PATH variable into directories.
// Check if the command is present in the directories:
// For each directory in PATH, call check_dir() to search for the command.
// If a command is found, call magic_box() to execute it.
// Return the exit status from magic_box().