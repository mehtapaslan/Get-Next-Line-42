/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mehaslan <mehaslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/23 01:19:23 by mehaslan          #+#    #+#             */
/*   Updated: 2026/11/24 02:44:35 by mehaslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static void	join_line(int fd, char **remainder)
{
    char	*buffer;
    int bytes;

    buffer = malloc(BUFFER_SIZE + 1);
    if (!buffer)
		return ;
    bytes = 1;
	while (bytes > 0 && !gnl_strchr(*remainder, '\n'))
	{
		bytes = read(fd, buffer, BUFFER_SIZE);
		if (bytes < 0)
		{
			free(*remainder);
			*remainder = NULL;
			break;
		}
		buffer[bytes] = '\0';
		*remainder = gnl_strjoin(*remainder, buffer);
        if (!*remainder)
            break;
	}
    free(buffer);
}

static char	*get_line(char *remainder)
{
    char *line;
    int len;
    int i;

    len = 0;
    i = 0;
    if(!remainder || !remainder[0])
        return(NULL);
    while(remainder[len] && remainder[len] != '\n')
        len++;
    if (remainder[len] == '\n')
		len++;
    line = malloc(sizeof(char) * (len + 1));
    if(!line)
        return(NULL);
    while(len--)
    {
        line[i] = remainder[i];
        i++;
    }  
    line[i] = '\0';
    return(line);
}

static void	next_line(char **remainder)
{
    
}

char *get_next_line(int fd)
{
    static char *remainder;
    char *line;

    if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
    join_line(fd, &remainder);
    if (!remainder)
		return (NULL);
    line = get_line(remainder);
    next_line(&remainder);
    return(line);
}