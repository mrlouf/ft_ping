/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nponchon <nponchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 14:22:02 by nponchon          #+#    #+#             */
/*   Updated: 2025/09/06 14:27:27 by nponchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/ft_ping.h"

extern t_ping g_ping;

void    ping_socket_init(void)
{
    if ((g_ping.ping_socket = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) < 0) {
        perror("socket");
        exit(1);
    }

    g_ping.ping_ident = getpid() & 0xFFFF;

    g_ping.ping_addr.sin_family = AF_INET;
    g_ping.ping_addr.sin_port = 0; // Not used in ICMP

    if (inet_pton(AF_INET, g_ping.ping_ip, &g_ping.ping_addr.sin_addr) <= 0) {
        fprintf(stderr, "Invalid IP address: %s\n", g_ping.ping_ip);
        exit(1);
    }

    // Set TTL (Time To Live)
    if (setsockopt(g_ping.ping_socket, IPPROTO_IP, IP_TTL,
                   &g_ping.ping_ttl, sizeof(g_ping.ping_ttl)) != 0) {
        perror("setsockopt");
        exit(1);
    }
}