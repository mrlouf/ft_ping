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

/*
    Initialise the raw socket for Internet Control Message Protocol (ICMP)
    communication. Sets up the socket, address structure,
    and necessary options like TTL, depending on the flags provided.
*/
void    ping_socket_init(void)
{
    if ((g_ping.ping_socket = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) < 0) {
        fprintf(stderr, "Socket creation failed: %s\n", strerror(errno));
        exit(1);
    }

    // Initialize the ping identifier with the process ID
    g_ping.ping_ident = getpid() & 0xFFFF;

    g_ping.ping_addr.sin_family = AF_INET;
    g_ping.ping_addr.sin_port = 0; // Ports are not used in ICMP

    if (inet_pton(AF_INET, g_ping.ping_ip, &g_ping.ping_addr.sin_addr) <= 0) {
        fprintf(stderr, "Invalid IP address: %s\n", g_ping.ping_ip);
        exit(1);
    }

    // Set TTL (Time To Live)
    if (setsockopt(g_ping.ping_socket, IPPROTO_IP, IP_TTL,
                   &g_ping.ping_ttl, sizeof(g_ping.ping_ttl)) != 0) {
        fprintf(stderr, "Error setting socket options: %s\n", strerror(errno));
        exit(1);
    }

    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 100000;
    if (setsockopt(g_ping.ping_socket, SOL_SOCKET, SO_RCVTIMEO,
                   &timeout, sizeof(timeout)) != 0) {
        fprintf(stderr, "Error setting socket options: %s\n", strerror(errno));
        exit(1);
    }
}