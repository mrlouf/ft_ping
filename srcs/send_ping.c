/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   send_ping.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nponchon <nponchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 14:21:55 by nponchon          #+#    #+#             */
/*   Updated: 2025/09/06 14:55:00 by nponchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/ft_ping.h"

extern t_ping g_ping;

void	ping_finish(void) {

	struct timeval end;
	gettimeofday(&end, NULL);
	double total_time = ((end.tv_sec - g_ping.ping_start.tv_sec) * 1000.0) +
		((end.tv_usec - g_ping.ping_start.tv_usec) / 1000.0);

	fflush(stdout);
	printf("\n");
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
	printf(", time %.0fms", total_time);
	printf("\n");

	double avg = 0, mdev = 0;
    if (g_ping.ping_rtt_count > 0) {
        // Calculate average
        for (size_t i = 0; i < g_ping.ping_rtt_count; ++i)
            avg += g_ping.ping_rtts[i];
        avg /= g_ping.ping_rtt_count;

        // Calculate mdev
        for (size_t i = 0; i < g_ping.ping_rtt_count; ++i)
            mdev += fabs(g_ping.ping_rtts[i] - avg);
        mdev /= g_ping.ping_rtt_count;
    }

	if (g_ping.ping_num_recv > 0) {
		printf("rtt min/avg/max/mdev = %0.3f/%0.3f/%0.3f/%0.3f ms\n",
			g_ping.ping_rtt_min,
			avg,
			g_ping.ping_rtt_max,
			mdev
		);
	}

	if (g_ping.ping_socket != -1)
		close(g_ping.ping_socket);
	free(g_ping.ping_fqdn);
	exit(0);
}

void handle_time_exceeded(struct icmphdr *icmp, struct iphdr *ip, ssize_t bytes_received, struct sockaddr_in addr) {
	char host[NI_MAXHOST];
	if (getnameinfo((struct sockaddr *)&addr, sizeof(addr), host, sizeof(host), NULL, 0, 0) == 0) {
		printf("%zd bytes from %s (%s): icmp_seq=%u Time to live exceeded\n",
			bytes_received - (ip->ihl * 4),
			host,
			inet_ntoa(addr.sin_addr),
			ntohs(icmp->un.echo.sequence));
		g_ping.ping_errs++;
	} else {
		printf("%zd bytes from %s: icmp_seq=%u Time to live exceeded\n",
			bytes_received - (ip->ihl * 4),
			inet_ntoa(addr.sin_addr),
			ntohs(icmp->un.echo.sequence));
		g_ping.ping_errs++;
	}
}

void	handle_echo_reply(struct icmphdr *icmp, struct iphdr *ip, ssize_t bytes_received) {
	g_ping.ping_num_recv++;

	struct timeval now;
	gettimeofday(&now, NULL);

	// Calculate RTT
	double rtt = ((now.tv_sec - g_ping.ping_time.tv_sec) * 1000.0) +
		((now.tv_usec - g_ping.ping_time.tv_usec) / 1000.0);
	rtt = round(rtt * 100.0) / 100.0;

	// Store RTT for statistics
	if (g_ping.ping_rtt_count < MAX_PINGS)
		g_ping.ping_rtts[g_ping.ping_rtt_count++] = rtt;

	if (rtt < g_ping.ping_rtt_min)
		g_ping.ping_rtt_min = rtt;
	if (rtt > g_ping.ping_rtt_max)
		g_ping.ping_rtt_max = rtt;

	printf("%zd bytes from %s (%s): icmp_seq=%u ttl=%d, time=%.2f ms\n",
		bytes_received - (ip->ihl * 4),
		g_ping.ping_fqdn,
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

static void calculate_rtt() {
	struct timeval now;
	gettimeofday(&now, NULL);
	size_t rtt = (now.tv_sec - g_ping.ping_time.tv_sec) * 1000 +
				 (now.tv_usec - g_ping.ping_time.tv_usec) / 1000;

	if (rtt < g_ping.ping_rtt_min)
		g_ping.ping_rtt_min = rtt;
	if (rtt > g_ping.ping_rtt_max)
		g_ping.ping_rtt_max = rtt;
}

/*
	Receive ICMP Echo Reply packets and process them. Handles timeouts and
	prints relevant information about the received packets or errors.
*/
void    ping_receive(void)
{
	char buffer[1024];
	struct sockaddr_in addr;
	socklen_t addr_len = sizeof(addr);
	ssize_t bytes_received;

	// Set socket to non-blocking with timeout
	struct timeval tv;
	tv.tv_sec = g_ping.ping_timeout;
	tv.tv_usec = 0;
	setsockopt(g_ping.ping_socket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);

	bytes_received = recvfrom(g_ping.ping_socket, buffer, sizeof(buffer), 0,
							(struct sockaddr *)&addr, &addr_len);

	if (bytes_received < 0) {
		if (errno == EAGAIN || errno == EWOULDBLOCK) {
			printf("Request timed out for icmp_seq=%u\n", g_ping.ping_seq_num);
			g_ping.ping_errs++;
		} else {
			fprintf(stderr, "Error receiving packet: %s\n", strerror(errno));
		}
		return;
	}

	// Parse IP and ICMP headers from the received packet
	struct iphdr *ip = (struct iphdr *)buffer;
	struct icmphdr *icmp = (struct icmphdr *)(buffer + (ip->ihl * 4));

	if (icmp->type == ICMP_ECHOREPLY && ntohs(icmp->un.echo.id) == g_ping.ping_ident) {
		handle_echo_reply(icmp, ip, bytes_received);
		calculate_rtt();
	} else if (icmp->type == ICMP_TIME_EXCEEDED) {
		handle_time_exceeded(icmp, ip, bytes_received, addr);
	} else if (icmp->type == ICMP_ECHOREPLY) {
		g_ping.ping_num_rept++;
		if (g_ping.ping_flag_v)
			printf("Duplicate packet received: icmp_seq=%u\n", ntohs(icmp->un.echo.sequence));
	} else {
		if (g_ping.ping_flag_v)
			printf("Received non-echo reply packet: type=%d code=%d\n", icmp->type, icmp->code);
	}
}

/*
	The main loop of the ping function. It sends ICMP Echo Request packets until
	the user stops the process or a certain number of packets (specified by -c flag) have been sent.
	It also handles receiving replies.
*/
void ping_send(void)
{
	gettimeofday(&g_ping.ping_start, NULL);

	printf("PING %s (%s) %zu(%zu) bytes of data\n",
		g_ping.ping_hostname,
		g_ping.ping_ip,
		g_ping.ping_data_len,
		g_ping.ping_data_len + sizeof(struct icmphdr) + sizeof(struct iphdr));

	// 56 bytes data by default, can be modified via the flag -s
	char packet[g_ping.ping_data_len + sizeof(struct icmphdr) + sizeof(struct iphdr)];
	struct icmphdr *icmp = (struct icmphdr *)packet;

	while (g_ping.ping_running) {
		memset(packet, 0, sizeof(packet));

		// Fill ICMP header
		icmp->type = ICMP_ECHO;
		icmp->code = 0;
		icmp->un.echo.id = htons(g_ping.ping_ident);
		icmp->un.echo.sequence = htons(++g_ping.ping_seq_num);

		// Fill payload with zeroes
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
			fprintf(stderr, "Error sending packet: %s\n", strerror(errno));
			continue;
		} else {
			g_ping.ping_num_emit++;
		}

		ping_receive();

		if (g_ping.ping_flag_c > 0 && (int)g_ping.ping_num_emit >= g_ping.ping_flag_c) {
			g_ping.ping_running = 0;
			break;
		}

		sleep(g_ping.ping_interval);
	}

	ping_finish();
}