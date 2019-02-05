/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grota <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/18 12:51:53 by grota             #+#    #+#             */
/*   Updated: 2019/02/05 17:15:44 by grota            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static int		ft_buf_manip(char **line, char **next,
												char **buf, unsigned int flag)
{
	if (flag & 1)
	{
		(void)buf;
		if (!next || !ft_isinstr(*next, '\n'))
			return (0);
		*line = ft_strsub(*next, 0, ft_strclen(*next, '\n'));
		*next = ft_strsub(*next, ft_strclen(*next, '\n') + 1,
													ft_strlen(*next));
		return (1);
	}
	else if (flag & 2)
	{
		*line = ft_strjoinf(*next, ft_strsub(*buf, 0,
												ft_strclen(*buf, '\n')), 3);
		*next = ft_strsubf(*buf, ft_strclen(*buf, '\n') + 1,
								BUFF_SIZE + 1);
		return (1);
	}
	return (1);
}

int				get_next_line(const int fd, char **line)
{
	static char		*gnl[4865];
	int				ret;
	char			*buf;

	if (fd < 0 || BUFF_SIZE < 1 || !line || !(buf = ft_strnew(BUFF_SIZE)))
		return (-1);
	if (ft_strlen(gnl[fd]) && ft_buf_manip(line, gnl + fd, &buf, 1))
		return (1);
	while ((ret = read(fd, buf, BUFF_SIZE)) && ret > 0)
	{
		if (ft_isinstr(buf, '\n'))
			return (ft_buf_manip(line, gnl + fd, &buf, 2));
		gnl[fd] = ft_strjoinf(gnl[fd], buf, 3);
		buf = ft_strnew(BUFF_SIZE);
	}
	if (ft_strlen(gnl[fd]))
	{
		*line = gnl[fd];
		gnl[fd] = NULL;
		return (1);
	}
	if (!ret)
		*line = NULL;
	return (ret);
}
