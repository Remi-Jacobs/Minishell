/**
 * redir - Handles output redirection to a specified file.
 * 
 * @shell_state: A pointer to the shell state structure (t_shell_state) containing file descriptors.
 * @token: A pointer to the token structure (t_cmd_token) containing the file name for redirection.
 * @type: An integer indicating the type of redirection (`TRUNC` for truncate, append otherwise).
 * 
 * This function manages the redirection of output to a specified file. It closes the 
 * existing output file descriptor and opens a new file for writing. Depending on the 
 * provided `type`, the file is opened in truncate mode (overwrite) or append mode.
 * 
 * If the file fails to open, an error message is displayed, and the return code and 
 * execution flag in `mini` are set accordingly. On success, the file descriptor is 
 * duplicated to `STDOUT` to redirect the output to the newly opened file.
 */

// Pseudo Code for redir
// Close the current output file descriptor (shell_state->output_fd).
// Check the type of redirection:
// If type is TRUNC:
// Open the file specified by token->str with write permissions and truncate it.
// Assign the file descriptor to shell_state->output_fd.
// Else (for append):
// Open the file specified by token->str with write permissions and append mode.
// Assign the file descriptor to shell_state->output_fd.
// Check if opening the file was successful:
// If the file failed to open (file descriptor is -1):
// Print an error message: "minishell: <filename>: No such file or directory".
// Set the return code (shell_state->return_code) to 1.
// Set the no_exec flag in mini to indicate that execution should not proceed.
// Exit the function.
// Duplicate the output file descriptor (shell_state->output_fd) to STDOUT.

/**
 * input - Handles input redirection from a specified file.
 * 
 * @shell_state: A pointer to the shell state structure (t_shell_state) containing file descriptors.
 * @token: A pointer to the token structure (t_cmd_token) containing the file name for redirection.
 * 
 * This function manages the redirection of input from a specified file. It closes the 
 * existing input file descriptor and opens the file in read-only mode. If the file fails 
 * to open, an error message is displayed, and the return code and execution flag in `shell_state` 
 * are set accordingly. On success, the file descriptor is duplicated to `STDIN` to redirect 
 * the input from the newly opened file.
 */

// Pseudo Code for input
// Close the current input file descriptor (shell_sate->input_fd).
// Open the file specified by token->content with read-only permissions.
// Check if opening the file was successful:
// If the file failed to open (file descriptor is -1):
// Print an error message: "minishell: <filename>: No such file or directory".
// Set the return code (shell_state->return_code) to 1.
// Set the no_exec flag in mini to indicate that execution should not proceed.
// Exit the function.
// Duplicate the input file descriptor (shell->input_fd) to STDIN.

/**
 * minipipe - Creates a pipe for inter-process communication and forks a new process.
 * 
 * @shell_state: A pointer to the shell state structure (t_shell_state) containing file descriptors and process details.
 * 
 * This function creates a pipe using two file descriptors: one for reading and one for writing. 
 * It forks the current process into a parent and a child. The child process closes the write end 
 * of the pipe, duplicates the read end to `STDIN`, and sets relevant state information in `shell_state`. 
 * It then returns a special code (`2`) indicating the child.
 * 
 * The parent process closes the read end of the pipe, duplicates the write end to `STDOUT`, and 
 * sets relevant state information in `shell_state`, including the process ID of the child. It then 
 * returns a special code (`1`) indicating the parent.
 * 
 * Return:
 * - 2 if the current process is the child process.
 * - 1 if the current process is the parent process.
 */

// Pseudo Code for minipipe
// Create a pipe with two file descriptors: pipefd[0] for reading and pipefd[1] for writing.
// Fork the current process:
// If in the child process (pid == 0):
// Close the write end of the pipe (pipefd[1]).
// Duplicate the read end of the pipe (pipefd[0]) to STDIN.
// Assign the read end of the pipe to shell_state->pipe_intput_fd.
// Set shell_state->proc_id to -1.
// Set shell_state->is_parent_proc to 0.
// Reset the no_exec flag in shell_state.
// Return 2 to indicate this is the child process.
// If in the parent process:
// Close the read end of the pipe (pipefd[0]).
// Duplicate the write end of the pipe (pipefd[1]) to STDOUT.
// Assign the write end of the pipe to shell_state->pipe_output_fd.
// Set shell_state->proc_id to the child's process ID.
// Set shell_state->last_exit_status to 0.
// Return 1 to indicate this is the parent process.