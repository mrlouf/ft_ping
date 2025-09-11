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

t_ping g_ping = {

	.ping_hostname = NULL,
	.ping_fqdn = NULL,
	.ping_ip = NULL,

	.is_root = 0,
	.ping_socket = -1,
	.ping_ident = 0,
	.ping_errs = 0,
	.ping_data_len = 56,
	.ping_interval = 1,
	.ping_timeout = 1,
	.ping_ttl = 64,

	.ping_seq_num = 0,
	.ping_num_emit = 0,
	.ping_num_recv = 0,
	.ping_num_rept = 0,
	.ping_running = 1,
	.ping_addr = {0},

	.ping_start = {0, 0},
	.ping_time = {0, 0},

	.ping_flag_v = 0,
	.ping_flag_c = -1,
	.ping_flag_q = 0,
};

void	sigint_handler(int sig) {
	(void)sig;
	g_ping.ping_running = 0;
	ping_finish();
}

int	main(int ac, char **av) {
	if (ac < 2) {
		dprintf(STDERR_FILENO, "ft_ping: usage error: Destination address required\n");
		exit(1);
	}

	setuid(0);
	// Check for root privileges
	if (getuid() != 0) {
		dprintf(STDERR_FILENO, "ft_ping: must be run as root\n");
		exit(1);
	}

	g_ping.is_root = 1;

	struct sigaction sa;
    sa.sa_handler = sigint_handler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGINT, &sa, NULL);

	ping_parse(ac, av);
	ping_socket_init();
	ping_send();

	return (0);
}