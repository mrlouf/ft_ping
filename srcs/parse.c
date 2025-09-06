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

void	display_help(void)
{
	printf("Usage: ft_ping [options] <destination>\n\n");
	printf("Options:\n");
	printf("  -c <count>    stop after sending (and receiving) <count> ECHO_RESPONSE packets\n");
	printf("  -v    	verbose output\n");
	printf("  -?    	display this help and exit\n");
}

void	get_flags(int ac, char **av)
{
	for (int i = 1; i < ac; i++) {
		if (strcmp(av[i], "-c") == 0 && i + 1 < ac) {
			g_ping.ping_flag_c = atoi(av[++i]);
			if (g_ping.ping_flag_c <= 0) {
				fprintf(stderr, "Invalid count value -- '%s'\n", av[i]);
				exit(1);
			}
		} else if (strcmp(av[i], "-v") == 0) {
			g_ping.ping_flag_v = 1;
		} else if (strcmp(av[i], "-?") == 0) {
			display_help();
			exit(0);
		} else if (av[i][0] == '-') {
			fprintf(stderr, "Invalid option -- '%s'\n\n", av[i]);
			display_help();
			exit(1);}
	}
}

void	resolve_hostname(void)
{
	struct addrinfo hints = {0}, *res;
    hints.ai_family = AF_INET; // IPv4 only; use AF_UNSPEC for both IPv4/6

    if (getaddrinfo(g_ping.ping_hostname, NULL, &hints, &res) == 0) {
        struct sockaddr_in *addr = (struct sockaddr_in *)res->ai_addr;
        static char ipstr[INET_ADDRSTRLEN];
		// Convert the IP to a string and store it
        inet_ntop(AF_INET, &(addr->sin_addr), ipstr, sizeof(ipstr));
        g_ping.ping_ip = ipstr;
		g_ping.ping_addr = *addr;

        freeaddrinfo(res);
    } else {
        g_ping.ping_ip = NULL;
		fprintf(stderr, "Could not resolve hostname \"%s\"\n", g_ping.ping_hostname);
		exit(1);
    }
}

void	ping_parse(int ac, char **av)
{
	get_flags(ac, av);

	g_ping.ping_hostname = av[ac - 1];
	g_ping.is_root = (getuid() == 0);

	resolve_hostname();

}