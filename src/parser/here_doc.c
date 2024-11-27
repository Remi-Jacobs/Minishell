/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ojacobs <ojacobs@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 15:45:39 by dsamuel           #+#    #+#             */
/*   Updated: 2024/11/27 04:16:37 by ojacobs          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_hd_sig_handler(int signal)
{
	(void)signal;
	write(2, "\n", 1);
	exit(130);
}

// static void	ft_here_doc_routine(char *delimiter, int temp_fd, t_shell_state *shell_state)
// {
// 	char	*input_line;

// 	(void)shell_state;
// 	signal(SIGINT, ft_hd_sig_handler);
// 	while (1)
// 	{
// 		input_line = readline(">  ");
// 		if (!input_line)
// 		{
// 			write(2, "minishell: warning: here-doc delimited 
// 			by end-of-file (wanted `", 60);
// 			write(2, delimiter, ft_strlen(delimiter));
// 			write(2, "`)\n", 3);
// 			break ;
// 		}
// 		if (ft_strcmp(input_line, delimiter) == 0)
// 		{
// 			free(input_line);
// 			break ;
// 		}
// 		write(temp_fd, input_line, ft_strlen(input_line));
// 		write(temp_fd, "\n", 1);
// 		free(input_line);
// 	}
// 	signal(SIGINT, SIG_DFL);
// }

char	*ft_strrstr(const char *haystack, const char *needle)
{
	const char	*last_occurrence = NULL;
	const char	*current_pos;
	size_t		needle_len;

	if (!*needle) // If needle is an empty string, return the full haystack
		return ((char *)haystack);

	needle_len = 0;
	while (needle[needle_len]) // Get length of needle
		needle_len++;

	current_pos = haystack;
	while (*current_pos) // Iterate through the haystack
	{
		// Check if current position in haystack starts with the needle
		if (*current_pos == *needle && !strncmp(current_pos, needle, needle_len))
			last_occurrence = current_pos; // Update last occurrence if match is found
		current_pos++;
	}
	return ((char *)last_occurrence); // Return the last occurrence found (or NULL if none found)
}
char	*ft_getenv(const char *var_name, t_env_variable *env)
{
	t_env_variable	*current_env;
	size_t			var_len;

	current_env = env;
	var_len = ft_strlen(var_name);

	while (current_env)
	{
		// Check if the current environment variable starts with the given variable name followed by '='
		if (ft_strncmp(current_env->variable, var_name, var_len) == 0
			&& current_env->variable[var_len] == '=')
		{
			// Return the value part (after the '=' sign)
			return (current_env->variable + var_len + 1);
		}
		current_env = current_env->next;
	}
	return (NULL); // Return NULL if the variable was not found
}

char	*ft_strjoin_free(char *s1, const char *s2)
{
	char	*new_str;
	size_t	len1;
	size_t	len2;

	if (!s1)
		s1 = ft_strdup(""); // If the first string is NULL, treat it as an empty string
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	new_str = malloc(len1 + len2 + 1);
	if (!new_str)
		return (NULL);
	ft_memcpy(new_str, s1, len1);      // Copy s1 to new_str
	ft_memcpy(new_str + len1, s2, len2); // Append s2 to new_str
	new_str[len1 + len2] = '\0';       // Null-terminate the new string
	free(s1);                          // Free the original s1
	return (new_str);
}

static int	is_quoted_delimiter(char *delimiter, t_shell_state *shell_state)
{
	char	*raw_input;
	char	*last_occurrence;
	int		in_quotes;
	int		last_index;

	// Ensure we get the actual last history entry that has content
	last_index = shell_state->history_count - 1;
	if (last_index < 0)
		return (0); // No history to check yet

	raw_input = shell_state->history[last_index]; // Get the last raw input from history
	if (!raw_input)
		return (0);
	last_occurrence = ft_strrstr(raw_input, "<<"); // Find the last occurrence of "<<"
	if (!last_occurrence)
		return (0);
	in_quotes = 0;
	while (*last_occurrence)
	{
		if (*last_occurrence == '"' || *last_occurrence == '\'') // Toggle quote state
			in_quotes = !in_quotes;
		if (ft_strncmp(last_occurrence, delimiter, ft_strlen(delimiter)) == 0)
			return (in_quotes); // Return true if delimiter is quoted
		last_occurrence++;
	}
	return (0); // Return false if not quoted
}

static char	*expand_input_line(char *input_line, t_shell_state *shell_state)
{
	char	*expanded_line;
	char	*var_value;
	int		i;

	expanded_line = ft_strdup("");
	i = 0;
	while (input_line[i])
	{
		if (input_line[i] == '$' && input_line[i + 1] && ft_isalpha(input_line[i + 1]))
		{
			int start = i + 1;
			while (input_line[i + 1] && (ft_isalnum(input_line[i + 1]) || input_line[i + 1] == '_'))
				i++;
			char *var_name = ft_substr(input_line, start, i - start + 1); // Extract variable name
			var_value = ft_getenv(var_name, shell_state->active_env);
			if (var_value)
				expanded_line = ft_strjoin_free(expanded_line, var_value); // Append expanded variable value
			else
				expanded_line = ft_strjoin_free(expanded_line, ""); // If variable is not found, append empty
			free(var_name);
		}
		else
		{
			char tmp[2] = { input_line[i], '\0' }; // Append the character as it is
			expanded_line = ft_strjoin_free(expanded_line, tmp);
		}
		i++;
	}
	return (expanded_line);
}

static void	ft_here_doc_routine(char *delimiter, int temp_fd, t_shell_state *shell_state)
{
	char	*input_line;
	char	*expanded_line;

	signal(SIGINT, ft_hd_sig_handler);
	while (1)
	{
		input_line = readline("> ");
		if (!input_line)
		{
			write(2, "minishell: warning: here-doc delimited by end-of-file (wanted `", 60);
			write(2, delimiter, ft_strlen(delimiter));
			write(2, "`)\n", 3);
			break ;
		}
		if (ft_strcmp(input_line, delimiter) == 0)
		{
			free(input_line);
			break ;
		}
		// If the delimiter is quoted in the raw input, skip variable expansion
		if (!is_quoted_delimiter(delimiter, shell_state))
			expanded_line = expand_input_line(input_line, shell_state);
		else
			expanded_line = ft_strdup(input_line); // No expansion if quoted
		write(temp_fd, expanded_line, ft_strlen(expanded_line));
		write(temp_fd, "\n", 1);
		free(input_line);
		free(expanded_line);
	}
	signal(SIGINT, SIG_DFL);
}

static void	ft_here_doc_parent(t_shell_state *shell_state, pid_t child_pid)
{
	int	signal_code;
	int	status;

	waitpid(child_pid, &status, 0);
	if (WIFSIGNALED(status))
	{
		signal_code = WTERMSIG(status);
		if (signal_code == SIGINT)
		{
			shell_state->return_code = 130;
			shell_state->should_skip_exec = 1;
			return ;
		}
	}
	else if (WIFEXITED(status))
		shell_state->return_code = WEXITSTATUS(status);
	shell_state->input_fd = open("here_doc", O_RDONLY);
	if (shell_state->input_fd == -1)
	{
		perror("Error opening temporary file for here-doc");
		return ;
	}
	dup2(shell_state->input_fd, STDIN_FILENO);
	ft_close(shell_state->input_fd);
}

void	ft_here_doc(t_shell_state *shell_state, t_cmd_token *token)
{
	char	*delimiter;
	int		temp_fd;
	pid_t	child_pid;

	g_global_sig.print_prompt = 1;
	delimiter = token->content;
	// print active envirmental vraiables
    // while (shell_state->active_env)
    // {
    //     printf("active envirmental variables: %s\n", shell_state->active_env->variable);
    //     shell_state->active_env = shell_state->active_env->next;
    // }
	child_pid = fork();
	if (child_pid == 0)
	{
		temp_fd = open("here_doc", O_CREAT | O_WRONLY | O_TRUNC, 0644);
		if (temp_fd == -1)
		{
			perror("Error creating temporary file for here-doc");
			exit(1);
		}
		ft_here_doc_routine(delimiter, temp_fd, shell_state);
		close(temp_fd);
		exit(0);
	}
	else if (child_pid > 0)
		ft_here_doc_parent(shell_state, child_pid);
	else
		perror("Error forking here-doc process");
}
