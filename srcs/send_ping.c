/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   send_ping.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicolas <nicolas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 14:21:55 by nponchon          #+#    #+#             */
/*   Updated: 2025/09/16 15:50:39 by nicolas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/ft_ping.h"

extern t_ping g_ping;

void	ping_finish(void) {

	struct timeval end;
	gettimeofday(&end, NULL);
/* 	double total_time = ((end.tv_sec - g_ping.ping_start.tv_sec) * 1000.0) +
		((end.tv_usec - g_ping.ping_start.tv_usec) / 1000.0); */

	fflush(stdout);
	printf("--- %s ping statistics ---\n", g_ping.ping_hostname);
	printf("%zu packets transmitted, ", g_ping.ping_num_emit);
	printf("%zu packets received, ", g_ping.ping_num_recv);
	if (g_ping.ping_errs)
		printf("+%zu errors, ", g_ping.ping_errs);
	if (g_ping.ping_num_rept)
		printf("+%zu duplicates, ", g_ping.ping_num_rept);
	if (g_ping.ping_num_emit) {
		if (g_ping.ping_num_recv > g_ping.ping_num_emit)
			printf("-- received more packets than emitted, something went wrong --");
		else
			printf ("%d%% packet loss",
				(int)(((g_ping.ping_num_emit - g_ping.ping_num_recv) * 100)/
				g_ping.ping_num_emit));
	}
	// printf(", time %.0fms", total_time);
	printf("\n");

    // Calculate average
    if (g_ping.ping_num_recv <= 0) {
        return ;
    } else {
        g_ping.ping_avg = (g_ping.ping_min + g_ping.ping_max) / 2;
        // Calculate standard deviation
        double sum = 0.0;
        for (size_t i = 0; i < g_ping.ping_num_recv && i < MAX_PINGS; i++) {
            sum += (g_ping.ping_rtt_arr[i] - g_ping.ping_avg) * (g_ping.ping_rtt_arr[i] - g_ping.ping_avg);
        }
        g_ping.ping_stddev = sqrt(sum / g_ping.ping_num_recv);
    }

    printf("round-trip min/avg/max/stddev = %.3f/%.3f/%.3f/%.3f ms\n",
		g_ping.ping_min,
		g_ping.ping_avg,
		g_ping.ping_max,
		g_ping.ping_stddev);

	if (g_ping.ping_socket != -1)
		close(g_ping.ping_socket);
	free(g_ping.ping_fqdn);
	exit(0);
}

void handle_time_exceeded(struct iphdr *ip, struct sockaddr_in addr, ssize_t bytes_received) {

	//  g_ping.ping_errs++;
    char host[NI_MAXHOST];

	if (g_ping.ping_flag_q) {
		return;
	} else if (getnameinfo((struct sockaddr *)&addr, sizeof(addr), host, sizeof(host), NULL, 0, 0) == 0) {
		printf("%zd bytes from %s (%s): Time to live exceeded\n",
			bytes_received - (ip->ihl * 4),
            host,
			inet_ntoa(addr.sin_addr));
	} else {
        printf("%zd bytes from %s: Time to live exceeded\n",
            bytes_received - (ip->ihl * 4),
            inet_ntoa(addr.sin_addr));
    }
    if (g_ping.ping_flag_v) {
        printf("IP Hdr Dump:\n");
        for (size_t i = 0; i < ip->ihl * 4; i++)
            printf("%02x ", ((unsigned char *)ip)[i]);
        printf("\n");
        printf("Vr HL TOS  Len   ID Flg  off TTL Pro  cks      Src      Dst     Data\n");
        printf(" 4  5  %02x %04x  %04x  %d %04x  %02x  %02x %04x %s %s \n",
            ip->tos, ntohs(ip->tot_len), ntohs(ip->id),
            (ntohs(ip->frag_off) & 0x4000) ? 1 : 0,
            ntohs(ip->frag_off) & 0x1FFF,
            ip->ttl, ip->protocol, ntohs(ip->check),
            inet_ntoa(*(struct in_addr *)&ip->saddr),
            inet_ntoa(*(struct in_addr *)&ip->daddr));
        printf("ICMP: type %d, code %d, size %zd, id 0x%04x, seq 0x%04x\n",
            ((unsigned char *)ip)[ip->ihl * 4],
            ((unsigned char *)ip)[ip->ihl * 4 + 1],
            bytes_received - (ip->ihl * 4),
            g_ping.ping_ident,
            g_ping.ping_seq_num);
    }
}

void handle_unreachable(struct icmphdr *icmp, struct iphdr *ip, ssize_t bytes_received, struct sockaddr_in addr) {
	g_ping.ping_errs++;
	if (g_ping.ping_flag_q)
		return;

	char host[NI_MAXHOST];
	if (getnameinfo((struct sockaddr *)&addr, sizeof(addr), host, sizeof(host), NULL, 0, 0) == 0) {
		printf("%zd bytes from %s: icmp_seq=%u Destination Host Unreachable\n",
			bytes_received - (ip->ihl * 4),
			inet_ntoa(addr.sin_addr),
			ntohs(icmp->un.echo.sequence));
	} else {
		printf("%zd bytes from %s: icmp_seq=%u Destination Host Unreachable\n",
			bytes_received - (ip->ihl * 4),
			inet_ntoa(addr.sin_addr),
			ntohs(icmp->un.echo.sequence));
	}
}

void	handle_echo_reply(struct icmphdr *icmp, struct iphdr *ip, ssize_t bytes_received) {

	fflush(stdout);
	g_ping.ping_num_recv++;
	if (g_ping.ping_flag_q)
		return;

	struct timeval now;
	gettimeofday(&now, NULL);

	// Calculate RTT
	float rtt = ((now.tv_sec - g_ping.ping_time.tv_sec) * 1000.0) +
		((now.tv_usec - g_ping.ping_time.tv_usec) / 1000.0);

    if (g_ping.ping_num_recv - 1 < MAX_PINGS)
        g_ping.ping_rtt_arr[g_ping.ping_num_recv - 1] = rtt;

    // Update min, max
    if (g_ping.ping_min == 0 || rtt < g_ping.ping_min) {
        g_ping.ping_min = rtt;
    }
    if (rtt > g_ping.ping_max) {
        g_ping.ping_max = rtt;
    }

	printf("%zd bytes from %s: icmp_seq=%u ttl=%d, time=%.3f ms\n",
		bytes_received - (ip->ihl * 4),
		g_ping.ping_ip,
		ntohs(icmp->un.echo.sequence),
		ip->ttl,
		rtt);
}

/*
	Calculate ICMP checksum. The checksum is the 16-bit value used in the ICMP header
	to verify the integrity of the whole ICMP message. It is computed by summing the
	16-bit words of the ICMP message and taking the one's complement of the sum.
	That's like checking if the label on a package matches its contents,
	ensuring that the data hasn't been altered or corrupted during transmission.
*/
unsigned short icmp_checksum(void *b, int len) {
	unsigned short  *buf = b;
	unsigned int    sum = 0;
	unsigned short  result;

	for (sum = 0; len > 1; len -= 2)
		sum += *buf++;
	if (len == 1)
		sum += *(unsigned char*)buf;
	sum = (sum >> 16) + (sum & 0xFFFF);
	sum += (sum >> 16);
	result = ~sum;
	return result;
}

/*
	Receive ICMP Echo Reply packets and process them. Handles timeouts and
	prints relevant information about the received packets or errors.
*/
void    ping_receive(void)
{
    char                buffer[1024];
    struct sockaddr_in  addr;
    socklen_t           addr_len = sizeof(addr);
    ssize_t             bytes_received;

    bytes_received = recvfrom(g_ping.ping_socket, buffer, sizeof(buffer), 0,
                            (struct sockaddr *)&addr, &addr_len);

    if (bytes_received > 0) {
        struct iphdr    *ip = (struct iphdr *)buffer;        
        struct icmphdr  *icmp = (struct icmphdr *)(buffer + (ip->ihl * 4));

        if (icmp->type == ICMP_TIME_EXCEEDED) {
            handle_time_exceeded(ip, addr, bytes_received);
        } else if (icmp->type == ICMP_DEST_UNREACH) {
            handle_unreachable(icmp, ip, bytes_received, addr);
        } else if (icmp->type == ICMP_ECHOREPLY && ntohs(icmp->un.echo.id) == g_ping.ping_ident) {
            uint16_t recv_seq = ntohs(icmp->un.echo.sequence);

            if (recv_seq <= g_ping.ping_seq_num) {
                handle_echo_reply(icmp, ip, bytes_received);
            } else if (recv_seq > g_ping.ping_seq_num) {
                g_ping.ping_num_rept++;
/*                 if (g_ping.ping_flag_v) {
                    printf("Future packet received: icmp_seq=%u (expected <= %u)\n", 
                           recv_seq, g_ping.ping_seq_num);
                } */
            }
		}
    } else {
        if (errno != EAGAIN && errno != EWOULDBLOCK) {
            if (!g_ping.ping_flag_q) {
                fprintf(stderr, "Error receiving packet: %s\n", strerror(errno));
            }
        }
    }
}

void ping_send(void)
{
    gettimeofday(&g_ping.ping_start, NULL);

    printf("PING %s (%s): %zu data bytes",
        g_ping.ping_hostname,
        g_ping.ping_ip,
        g_ping.ping_data_len);
    if (g_ping.ping_flag_v) {
        printf(", id %#x = %d", g_ping.ping_ident, g_ping.ping_ident);
    }
    printf("\n");

    char packet[g_ping.ping_data_len + sizeof(struct icmphdr) + sizeof(struct iphdr)];
    struct icmphdr *icmp = (struct icmphdr *)packet;

    int is_localhost = (strcmp(g_ping.ping_ip, "127.0.0.1") == 0);

    while (g_ping.ping_running) {
        memset(packet, 0, sizeof(packet));

        // Fill ICMP header
        icmp->type = ICMP_ECHO;
        icmp->code = 0;
        icmp->un.echo.id = htons(g_ping.ping_ident);
        icmp->un.echo.sequence = htons(g_ping.ping_seq_num);
        icmp->checksum = 0;
        icmp->checksum = icmp_checksum(packet, sizeof(struct icmphdr) + g_ping.ping_data_len);

        gettimeofday(&g_ping.ping_time, NULL);
        
        ssize_t sent = sendto(
            g_ping.ping_socket,
            packet,
            sizeof(struct icmphdr) + g_ping.ping_data_len,
            0,
            (struct sockaddr *)&g_ping.ping_addr,
            sizeof(g_ping.ping_addr)
        );
        
        if (sent < 0) {
/*             if (!g_ping.ping_flag_q) {
                fprintf(stderr, "Error sending packet: %s\n", strerror(errno));
            } */
            g_ping.ping_errs++;
        } else {
            g_ping.ping_num_emit++;
        }

        fd_set          read_fds;
        struct timeval  tv;
        int             packets_received = 0;
        
        int max_packets = is_localhost ? 10 : 1;
        
        while (packets_received < max_packets) {
            FD_ZERO(&read_fds);
            FD_SET(g_ping.ping_socket, &read_fds);
            
            tv.tv_sec = is_localhost ? 0 : g_ping.ping_timeout;
            tv.tv_usec = is_localhost ? 100000 : 0; // 100ms for localhost
            
            int ready = select(g_ping.ping_socket + 1, &read_fds, NULL, NULL, &tv);
            
            if (ready > 0 && FD_ISSET(g_ping.ping_socket, &read_fds)) {
                size_t old_recv_count = g_ping.ping_num_recv;
                ping_receive();
                if (g_ping.ping_num_recv > old_recv_count) {
                    break;
                }
                packets_received++;
            } else if (ready == 0) {
                if (packets_received == 0) {
                    g_ping.ping_errs++;
/*                     if (!g_ping.ping_flag_q) {
                        printf("Request timeout for icmp_seq=%u\n", g_ping.ping_seq_num);
                    } */
                }
                break;
            } else {
                if (errno != EINTR) {
                    if (!g_ping.ping_flag_q) {
                        fprintf(stderr, "Error in select: %s\n", strerror(errno));
                    }
                    g_ping.ping_errs++;
                }
                break;
            }
        }

        if (g_ping.ping_flag_c > 0 && (int)g_ping.ping_num_emit >= g_ping.ping_flag_c) {
            g_ping.ping_running = 0;
            break;
        }

        if (g_ping.ping_running) {
            sleep(g_ping.ping_interval);
        }
        g_ping.ping_seq_num++;
    }

    ping_finish();
}