/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsamuel <dsamuel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 15:45:39 by dsamuel           #+#    #+#             */
/*   Updated: 2024/11/22 20:31:57 by dsamuel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <termios.h>

static void	ft_hd_sig_handler(int signal)
{
	(void)signal;
	write(2, "\n", 1);
	exit(130);
}

static void	ft_here_doc_routine(char *delimiter, int temp_fd)
{
	char	*input_line;

	signal(SIGINT, ft_hd_sig_handler);
	while (1)
	{
		input_line = readline("here_doc > ");
		if (!input_line)
		{
			write(2, "minishell: warning: here-doc delimited \
			by end-of-file (wanted `", 60);
			write(2, delimiter, ft_strlen(delimiter));
			write(2, "`)\n", 3);
			break ;
		}
		if (ft_strcmp(input_line, delimiter) == 0)
		{
			free(input_line);
			break ;
		}
		write(temp_fd, input_line, ft_strlen(input_line));
		write(temp_fd, "\n", 1);
		free(input_line);
	}
	signal(SIGINT, SIG_DFL);
}

void	ft_here_doc(t_shell_state *shell_state, t_cmd_token *token)
{
	char	*delimiter;
	int		temp_fd;
	int		status;
	pid_t	child_pid;

	if (!token)
	{
		write(2, "nothing in token\n", 18);
		return ;
	}
	delimiter = token->content;
	child_pid = fork();
	if (child_pid == 0)
	{
		temp_fd = open("here_doc", O_CREAT | O_WRONLY | O_TRUNC, 0644);
		if (temp_fd == -1)
		{
			perror("Error creating temporary file for here-doc");
			exit(1);
		}
		ft_here_doc_routine(delimiter, temp_fd);
		close(temp_fd);
		exit(0);
	}
	else if (child_pid > 0)
	{
		waitpid(child_pid, &status, 0);
		if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
		{
			shell_state->should_skip_exec = 1;
			return ;
		}
		shell_state->input_fd = open("here_doc", O_RDONLY);
		if (shell_state->input_fd == -1)
		{
			perror("Error opening temporary file for here-doc");
			return ;
		}
		dup2(shell_state->input_fd, STDIN);
		ft_close(shell_state->input_fd);
	}
	else
	{
		perror("Error forking here-doc process");
	}
}
