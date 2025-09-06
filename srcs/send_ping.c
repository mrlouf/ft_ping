#include "../incs/ft_ping.h"

extern t_ping g_ping;

void	ping_finish(void) {

	fflush(stdout);
	printf("\n");
	printf("--- %s ping statistics ---\n", g_ping.ping_hostname);
	printf("%zu packets emitted, ", g_ping.ping_num_emit);
	printf("%zu packets received, ", g_ping.ping_num_recv);
	if (g_ping.ping_num_rept)
		printf("+%zu duplicates, ", g_ping.ping_num_rept);
	if (g_ping.ping_num_emit) {
		if (g_ping.ping_num_recv > g_ping.ping_num_emit)
			printf("-- received more packets than emitted, something went wrong --");
		else
			printf ("%d%% packet loss",
				(int)(((g_ping.ping_num_emit - g_ping.ping_num_recv) * 100) /
				g_ping.ping_num_emit));
	}
	printf("\n");

    free(g_ping.ping_fqdn);
	exit(0);
}

void	ping_send(void)
{
	printf("PING %s (%s) %zu bytes of data\n",
        g_ping.ping_hostname, g_ping.ping_ip, g_ping.ping_data_len);

    while (g_ping.ping_running) {

		g_ping.ping_num_emit++;
        printf("%zu bytes from %s (%s): ",
            g_ping.ping_data_len, g_ping.ping_fqdn, g_ping.ping_ip);
        printf("icmp_seq=%u ttl=%d time=%.2f ms\n",
            ++g_ping.ping_seq_num, g_ping.ping_ttl, (double)(rand() % 1000) / 1000); // TODO: track actual time

        if (g_ping.ping_flag_c > 0 && (int)g_ping.ping_num_emit >= g_ping.ping_flag_c) {
            g_ping.ping_running = 0;
        }

        sleep(g_ping.ping_interval);
	}

    ping_finish();
    
}