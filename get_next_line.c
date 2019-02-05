/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grota <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/18 12:51:53 by grota             #+#    #+#             */
/*   Updated: 2019/02/05 14:40:44 by grota            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static int		ft_buf_manip(char **line, t_gnl *gnl, char **buf, unsigned int flag)
{
	if (flag & 1)
	{
		(void)buf;
		if (!(gnl->line) || !ft_isinstr(gnl->line, '\n'))
			return (0);
		*line = ft_strsubf(gnl->line, 0, ft_strclen(gnl->line, '\n'));
		gnl->line = ft_strsubf(gnl->line, ft_strclen(gnl->line, '\n') + 1, ft_strlen(gnl->line));
		return (1);
	}
	else if (flag & 2)
	{
		*line = ft_strjoinf(gnl->line, ft_strsub(*buf, 0, ft_strclen(*buf, '\n')), 2);
		gnl->line = ft_strsubf(*buf, ft_strclen(*buf, '\n') + 1, BUFF_SIZE + 1);
		return (1);
	}
	return (1);
}

static t_gnl	*ft_newlink(const int fd, t_gnl *gnl)
{
	if (!gnl)
	{
		if (!(gnl = (t_gnl *)malloc(sizeof(*gnl))))
			return (NULL);
		gnl->first = gnl;
	}
	else
	{
		while (gnl->next && gnl->fd != fd)
			gnl = gnl->next;
		if (gnl->fd == fd)
			return (gnl);
		if (!(gnl->next = (t_gnl *)malloc(sizeof(*gnl))))
			return (NULL);
		gnl->next->first = gnl->first;
		gnl = gnl->next;
	}
	gnl->fd = fd;
	gnl->line = NULL;
	gnl->next = NULL;
	return (gnl);
}

static t_gnl	*ft_select_fd(const int fd, t_gnl *gnl)
{
	if (!(gnl = ft_newlink(fd, gnl)))
		return (0);
	gnl = gnl->first;
	while (fd != gnl->fd && gnl->next)
		gnl = gnl->next;
	if (fd != gnl->fd && gnl->next == NULL)
		if (ft_newlink(fd, gnl) == 0)
			return (0);
	return (gnl);
}

int				get_next_line(const int fd, char **line)
{
	static t_gnl	*gnl = NULL;
	int				ret;
	char			*buf;

	if (fd < 0 || BUFF_SIZE < 1 || !line || !(buf = ft_strnew(BUFF_SIZE)))
		return (-1);
	if (!(gnl = ft_select_fd(fd, gnl)))
		return (-1);
	if (ft_strlen(gnl->line) && ft_buf_manip(line, gnl, &buf, 1))
		return (1);
	while ((ret = read(fd, buf, BUFF_SIZE)) && ret > 0)
	{
		if (ft_isinstr(buf, '\n'))
		{
			return (ft_buf_manip(line, gnl, &buf, 2));
		}
		gnl->line = ft_strjoinf(gnl->line, buf, 3);
		buf = ft_strnew(BUFF_SIZE);
	}
	if (ft_strlen(gnl->line))
	{
		*line = gnl->line;
		gnl->line = NULL;
		return (1);
	}
	if (!ret)
	{
		*line = NULL;
		free(gnl->line);
	}
	return (ret);
}

int	main(int argc, char **argv)
{
	int	i = 1;
	int	fd_1 = 0;
	int	fd_2 = 0;
	char	*line;

	line = ft_strnew(12);
	if (argc >= 2)
		fd_1 = open(argv[1], O_RDONLY);
	if (argc == 3)
		fd_2 = open(argv[2], O_RDONLY);
	while ((i = get_next_line(fd_1, &line)))
	{
		ft_putendl(line);
		i = get_next_line(fd_2, &line);
		ft_putendl(line);
	}
	read(0, O_RDONLY, BUFF_SIZE);
	return (1);
}
