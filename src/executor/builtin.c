/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsamuel <dsamuel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 21:22:53 by ojacobs           #+#    #+#             */
/*   Updated: 2024/10/29 13:03:57 by dsamuel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


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

// int ft_is_builtin(char *command)
// {
//     // Check if the command is one of the known built-in shell commands
//     if (ft_strcmp(command, "echo") == 0)
//         return 1;
//     if (ft_strcmp(command, "cd") == 0)
//         return 1;
//     if (ft_strcmp(command, "pwd") == 0)
//         return 1;
//     if (ft_strcmp(command, "env") == 0)
//         return 1;
//     if (ft_strcmp(command, "export") == 0)
//         return 1;
//     if (ft_strcmp(command, "unset") == 0)
//         return 1;

//     // If no match is found, return 0
//     return 0;
// }

int		ft_is_builtin(char *command)
{
	if (ft_strcmp(command, "echo") == 0)
		return (1);
	if (ft_strcmp(command, "cd") == 0)
		return (1);
	if (ft_strcmp(command, "pwd") == 0)
		return (1);
	if (ft_strcmp(command, "env") == 0)
		return (1);
	if (ft_strcmp(command, "export") == 0)
		return (1);
	if (ft_strcmp(command, "unset") == 0)
		return (1);
	return (0);
}

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

// int ft_exec_builtin(char **args, t_shell_state *shell_state)
// {
//     int result = 0;

//     // Check the first argument to determine the built-in command
//     if (ft_strcmp(args[0], "echo") == 0)
//         result = ft_echo(args);
//     else if (ft_strcmp(args[0], "cd") == 0)
//         result = ft_cd(args, shell_state->active_env);
//     else if (ft_strcmp(args[0], "pwd") == 0)
//         result = ft_pwd();
//     else if (ft_strcmp(args[0], "env") == 0)
//         result = ft_env(shell_state->active_env);
//     else if (ft_strcmp(args[0], "export") == 0)
//         result = ft_export(args, shell_state->active_env, shell_state->secret_env);
//     else if (ft_strcmp(args[0], "unset") == 0)
//         result = ft_unset(args, shell_state);
//     else
//         printf("Command not found: %s\n", args[0]);

//     return result;
// }

int		ft_exec_builtin(char **args, t_shell_state *shell_state)
{
	int		result;

	result = 0;
	if (ft_strcmp(args[0], "echo") == 0)
		result = ft_echo(args);
	if (ft_strcmp(args[0], "cd") == 0)
		result = ft_cd(args, shell_state->active_env);
	if (ft_strcmp(args[0], "pwd") == 0)
		result = ft_pwd();
	if (ft_strcmp(args[0], "env") == 0)
		ft_env(shell_state->active_env);
	if (ft_strcmp(args[0], "export") == 0)
		ft_export(args, shell_state->active_env, shell_state->secret_env);
	if (ft_strcmp(args[0], "unset") == 0)
		ft_unset(args, shell_state);
	return (result);
}