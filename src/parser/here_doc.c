/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ojacobs <ojacobs@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 15:45:39 by dsamuel           #+#    #+#             */
/*   Updated: 2024/11/14 17:55:45 by ojacobs          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Implements the here-doc (<<) feature for minishell.
 *
 * This function handles input redirection where the shell reads input
 * from the user until a specified delimiter is encountered. The input
 * is stored in a temporary file which is then used as the input for
 * subsequent command execution.
 *
 * Workflow:
 * 1. Create a temporary file for storing the here-doc input.
 * 2. Read user input line by line and write it to the temporary file.
 * 3. Stop reading when the delimiter is encountered.
 * 4. Redirect the shell's input to read from the temporary file.
 *
 * @param shell_state A pointer to the `t_shell_state` structure.
 * @param token A pointer to the current command token.
 */

void	ft_here_doc(t_shell_state *shell_state, t_cmd_token *token)
{
	char *delimiter;
	char *input_line;
	int temp_fd;
	
	// Get the delimiter from the next token content
	if (!token)
	{
		write(2,"nothing in token\n",18);
		return ;
	}
	delimiter = token->content;
	printf("Delimiter: %s\n", delimiter);
	
	// Create a temporary file for the here-doc
	temp_fd = open("here_doc", O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (temp_fd == -1)
	{
		perror("Error creating temporary file for here-doc");
		return;
	}

	// Prompt and read input until the delimiter is encountered
	while (1)
	{
		input_line = readline("here_doc > ");
		if (!input_line)
		{
			fprintf(stderr, "minishell: warning: here-doc delimited by end-of-file (wanted `%s`)\n", delimiter);
			break;
		}
		// Check if the input matches the delimiter
		if (strcmp(input_line, delimiter) == 0)
		{
			free(input_line);
			break;
		}
		// Write the input line to the temporary file
		dprintf(temp_fd, "%s\n", input_line);
		free(input_line);
	}

	// Close the temporary file after writing
	close(temp_fd);

	// Open the temporary file for reading
	shell_state->input_fd = open("here_doc", O_RDONLY);
	if (shell_state->input_fd == -1)
	{
		perror("Error opening temporary file for here-doc");
		return;
	}

	// Redirect standard input to the temporary file
	dup2(shell_state->input_fd, STDIN);
	ft_close(shell_state->input_fd);
}
