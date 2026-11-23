/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mehaslan <mehaslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/23 01:19:23 by mehaslan          #+#    #+#             */
/*   Updated: 2026/11/23 03:54:38 by mehaslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static void	join_line(int fd, char **remainder)
{
    
}

static char	*get_line(char *remainder)
{
    
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