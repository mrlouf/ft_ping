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
	exit(0);
}

void	ping_send(void)
{
	printf("PING %s (%s) %zu bytes\n",
        g_ping.ping_hostname, g_ping.ping_ip, g_ping.ping_data_len);

    while (g_ping.ping_running) {
		// Placeholder for ping sending logic
		sleep(g_ping.ping_interval);
		g_ping.ping_num_emit++;
		printf("Ping %zu sent to %s\n", g_ping.ping_num_emit, g_ping.ping_hostname);

        if (g_ping.ping_flag_c > 0 && (int)g_ping.ping_num_emit >= g_ping.ping_flag_c) {
            g_ping.ping_running = 0;
        }
	}

    ping_finish();
    
}