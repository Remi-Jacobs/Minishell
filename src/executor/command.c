/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ojacobs <ojacobs@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 22:25:18 by ojacobs           #+#    #+#             */
/*   Updated: 2024/10/27 21:51:54 by ojacobs          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int error_message(const char *path)
{
    struct stat path_stat;
    DIR *dir;
    int file_descriptor;

    // First, check if the path contains a '/'
    if (ft_strchr(path, '/') == NULL)
    {
        // If no '/', it's likely a command, so print "command not found"
        ft_putstr_fd("mimishell: ",STDERR);
        ft_putstr_fd((char *) path, STDERR);
        ft_putendl_fd(": command not found",STDERR);
        // fprintf(stderr, "minishell: %s: command not found\n", path);
        return UNKNOWN_CMD;
    }

    // Try to open the file at the given path
    file_descriptor = open(path, O_RDONLY);
    if (file_descriptor == -1)
    {
        // Check if the path is a directory
        dir = opendir(path);
        if (dir)
        {
            // Path is a directory, print "is a directory" error
            fprintf(stderr, "minishell: %s: is a directory\n", path);
            closedir(dir);
            return IS_DIRECTORY;
        } 
        else if (errno == ENOENT) {
            // If the file does not exist, print "No such file or directory"
            fprintf(stderr, "minishell: %s: No such file or directory\n", path);
        } 
        else if (errno == EACCES) {
            // If the file exists but permission is denied, print "Permission denied"
            fprintf(stderr, "minishell: %s: Permission denied\n", path);
        } 
        else {
            // Generic error case
            fprintf(stderr, "minishell: %s: Error\n", path);
        }
        return UNKNOWN_CMD;
    }

    // Check if the path is a directory
    if (fstat(file_descriptor, &path_stat) == 0 && S_ISDIR(path_stat.st_mode)) {
        // If it's a directory, print "is a directory"
        fprintf(stderr, "minishell: %s: is a directory\n", path);
        close(file_descriptor);
        return IS_DIRECTORY;
    }

    // Close the file descriptor if opened
    close(file_descriptor);

    // Return UNKNOWN_COMMAND if none of the above checks matched
    return UNKNOWN_CMD;
}


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

// Helper function to convert environment list to an array of strings
char **convert_env_to_array(t_env_variable *env)
{
    int count = 0;
    t_env_variable *tmp = env;
    char **env_array;

    // Count the number of environment variables
    while (tmp)
    {
        count++;
        tmp = tmp->next;
    }

    // Allocate memory for the environment array
    env_array = (char **)malloc(sizeof(char *) * (count + 1));
    if (!env_array)
        return NULL;

    // Copy environment variables to the array
    count = 0;
    while (env)
    {
        env_array[count++] = env->variable;
        env = env->next;
    }
    env_array[count] = NULL;  // Null-terminate the array
    return env_array;
}

int magic_box(const char *path, char **args, t_env_variable *env, t_shell_state *shell_state)
{
    pid_t pid;
    int status;
    char **envp;
    (void) shell_state;

    // Convert environment variables to an array of strings (envp)
    if(!(envp = convert_env_to_array(env)))
        return (ft_putendl_fd("failed to convert env to array", STDERR), -1); // You need to implement this function

    // Fork the process
    pid = fork();

    if (pid == 0)  // Child process
    {
        // In child process: Handle signals differently
        global_sig.child_proc_id = 1;

        // If the path contains a '/', try to execute the command using execve()
        if (ft_strchr(path, '/'))
        {
            if (execve(path, args, envp) == -1)
            {
                error_message(path);
                ft_free_tab(envp);
                exit(UNKNOWN_CMD);  // Exit with the custom error code
            }
        }

        // Free the environment array and exit
        ft_free_tab(envp);
        exit(EXIT_SUCCESS);
    }
    else if (pid > 0)  // Parent process
    {
        // In parent process: Wait for the child to finish and check signals
        global_sig.child_proc_id = pid;

        waitpid(pid, &status, 0);

        // Reset child process id after child finishes
        global_sig.child_proc_id = 0;

        if (WIFSIGNALED(status))  // If the process was terminated by a signal
        {
            if (WTERMSIG(status) == SIGINT)
                global_sig.last_exit_stat = 130;  // SIGINT exit status
            else if (WTERMSIG(status) == SIGQUIT)
                global_sig.last_exit_stat = 131;  // SIGQUIT exit status

            return global_sig.last_exit_stat;
        }

        // Check if the child exited normally
        if (WIFEXITED(status))
            global_sig.last_exit_stat = WEXITSTATUS(status);

        return global_sig.last_exit_stat;
    }
    else
    {
        // Fork failed
        perror("fork");
        return -1;
    }
}

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

char *path_join(const char *s1, const char *s2)
{
    char *joined_path;
    int len1, len2;

    if (!s1 || !s2)
        return (NULL);

    len1 = strlen(s1);
    len2 = strlen(s2);

    // Allocate memory for the new path (+2 for '/' and '\0')
    joined_path = (char *)malloc(len1 + len2 + 2);
    if (!joined_path)
        return (NULL);

    // Copy the first path (s1) into joined_path
    strcpy(joined_path, s1);

    // Add the forward slash if s1 does not already end with '/'
    if (s1[len1 - 1] != '/')
        strcat(joined_path, "/");

    // Append the second path (s2)
    strcat(joined_path, s2);

    return (joined_path);
}

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

char *check_dir(const char *bin, const char *command)
{
    DIR *dir;
    struct dirent *entry;
    char *full_path = NULL;

    // Open the directory specified by bin
    dir = opendir(bin);
    if (!dir)
        return (NULL);  // If directory cannot be opened, return NULL

    // Iterate through the directory entries
    while ((entry = readdir(dir)) != NULL)
    {
        // Check if the entry matches the command
        if (strcmp(entry->d_name, command) == 0)
        {
            // If found, join the bin path with the command name to create the full path
            full_path = path_join(bin, command);
            break;
        }
    }

    closedir(dir);  // Close the directory
    return (full_path);  // Return the full path if found, NULL otherwise
}

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

int exec_bin(char **args, t_env_variable *env, t_shell_state *shell_state)
{
    char *path_var;
    char **path_dirs;
    char *full_path;
    int i;

    // Search for the PATH environment variable
    path_var = getenv("PATH");
    if (!path_var)
    {
        // If PATH is not found, execute the command directly using magic_box
        return magic_box(args[0], args, env, shell_state);
    }

    // Split the PATH variable into directories using ':' as the delimiter
    path_dirs = ft_split(path_var, ':');
    if (!path_dirs)
        return -1;  // Error splitting the PATH

    // Check if the command is present in any of the directories
    i = 0;
    while (path_dirs[i])
    {
        // Search for the command in the current directory
        full_path = check_dir(path_dirs[i], args[0]);
        if (full_path)
        {
            // If the command is found, execute it using magic_box
            int result = magic_box(full_path, args, env, shell_state);
            free(full_path);
            free(path_dirs);
            return result;
        }
        i++;
    }

    // If no command is found in the PATH, call magic_box with the original command name
    free(path_dirs);
    return magic_box(args[0], args, env, shell_state);
}