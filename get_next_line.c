/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mehaslan <mehaslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/23 01:19:23 by mehaslan          #+#    #+#             */
/*   Updated: 2026/11/24 03:50:34 by mehaslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static void	join_line(int fd, char **remainder)
{
	char	*buffer;
	int		bytes;

	buffer = malloc(BUFFER_SIZE + 1);
	if (!buffer)
	{
		gnl_free(remainder);
		return ;
	}
	bytes = 1;
	while (bytes > 0 && !gnl_strchr(*remainder, '\n'))
	{
		bytes = read(fd, buffer, BUFFER_SIZE);
		if (bytes < 0)
		{
			gnl_free(remainder);
			break ;
		}
		buffer[bytes] = '\0';
		*remainder = gnl_strjoin(*remainder, buffer);
		if (!*remainder)
			break ;
	}
	free(buffer);
}

static char	*get_line(char *remainder)
{
	char	*line;
	int		len;
	int		i;

	len = 0;
	i = 0;
	if (!remainder || !remainder[0])
		return (NULL);
	while (remainder[len] && remainder[len] != '\n')
		len++;
	if (remainder[len] == '\n')
		len++;
	line = malloc(len + 1);
	if (!line)
		return (NULL);
	while (len--)
	{
		line[i] = remainder[i];
		i++;
	}
	line[i] = '\0';
	return (line);
}

static void	next_line(char **remainder)
{
	int		i;
	char	*newline;
	char	*temp;

	i = 0;
	newline = gnl_strchr(*remainder, '\n');
	if (!newline || !newline[1])
	{
		gnl_free(remainder);
		return ;
	}
	temp = malloc(gnl_strlen(newline + 1) + 1);
	if (!temp)
	{
		gnl_free(remainder);
		return ;
	}
	while (newline[1 + i])
	{
		temp[i] = newline[1 + i];
		i++;
	}
	temp[i] = '\0';
	free(*remainder);
	*remainder = temp;
}

char	*get_next_line(int fd)
{
	static char	*remainder;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	join_line(fd, &remainder);
	if (!remainder)
		return (NULL);
	line = get_line(remainder);
	next_line(&remainder);
	return (line);
}
