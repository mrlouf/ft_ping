#include "../incs/ft_ping.h"

extern t_ping g_ping;

void	ping_send(void)
{
	// Placeholder for ping sending logic
	printf("PING %s\n (%zu bytes)\n", g_ping.ping_hostname, g_ping.ping_data_len);
}