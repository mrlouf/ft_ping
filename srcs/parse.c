/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nponchon <nponchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 18:28:49 by nponchon          #+#    #+#             */
/*   Updated: 2025/09/05 18:32:37 by nponchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/ft_ping.h"

extern t_ping g_ping;

void	ping_parse(int ac, char **av)
{
	printf("parsing %s\n", av[ac - 1]);
	g_ping.ping_hostname = av[ac - 1];

	g_ping.is_root = (getuid() == 0);

	//TODO: resolve hostname to IP and store in g_ping.ping_ip
	//TODO: handle other command line arguments and set flags in g_ping


}