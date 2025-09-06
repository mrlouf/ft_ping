/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nponchon <nponchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 18:03:56 by nponchon          #+#    #+#             */
/*   Updated: 2025/09/05 19:25:00 by nponchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/ft_ping.h"

/*
** Global ping structure
*/
t_ping g_ping = {
	.args = NULL,
	.ping_hostname = NULL,
	.ping_ip = NULL,
	.ping_socket = -1,
	.ping_ident = 0,
	.ping_seq_num = 0,
	.ping_data_len = 56,
	.ping_interval = 1,
	.ping_timeout = 1,
	.ping_ttl = 64,
	.ping_flag_a = 0,
	.ping_num_xmit = 0,
	.ping_num_recv = 0,
	.ping_num_rept = 0,
	.ping_running = 1,
	.ping_addr = {0}
};

void	ping_finish(int sig) {

	(void)sig;
	fflush(stdout);
	printf("\n");
	printf("--- %s ping statistics ---\n", g_ping.ping_hostname);
	printf("%zu packets transmitted, ", g_ping.ping_num_xmit);
	printf("%zu packets received, ", g_ping.ping_num_recv);
	if (g_ping.ping_num_rept)
		printf("+%zu duplicates, ", g_ping.ping_num_rept);
	if (g_ping.ping_num_xmit) {
		if (g_ping.ping_num_recv > g_ping.ping_num_xmit)
			printf("-- somebody is printing forged packets!");
		else
			printf ("%d%% packet loss",
				(int)(((g_ping.ping_num_xmit - g_ping.ping_num_recv) * 100) /
				g_ping.ping_num_xmit));
	}
	printf("\n");
	exit(0);
}

int	main(int ac, char **av) {
	if (ac < 2) {
		char *err = "Usage: ./ft_ping <IPv4> opt:<flags>\n";
		write(2, err, strlen(err));
		exit(1);
	}

	struct sigaction sa;
    sa.sa_handler = ping_finish;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGINT, &sa, NULL);

	ping_parse(av);
	ping_initialise(av, &g_ping);

	while (true) {
		// Placeholder for ping sending logic
		sleep(g_ping.ping_interval);
		g_ping.ping_num_xmit++;
		printf("Ping %zu sent to %s\n", g_ping.ping_num_xmit, g_ping.ping_hostname);
	}

	return (0);
}