/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ping.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicolas <nicolas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 18:03:37 by nponchon          #+#    #+#             */
/*   Updated: 2025/09/16 15:50:22 by nicolas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PING_H
# define FT_PING_H

// Libraries/Headers

# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <stdio.h>
# include <signal.h>
# include <stdbool.h>
# include <limits.h>
# include <stddef.h>
# include <stdint.h>
# include <fcntl.h>

# include <errno.h>
# include <time.h>
# include <sys/time.h>

# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <netdb.h>
# include <netinet/ip_icmp.h>

# include <math.h>

// Macros

# define MAX_PINGS 1024

// Typedefs

typedef struct s_ping
{
	char	*ping_hostname;
	char	*ping_fqdn;
	char	*ping_ip;

	int		is_root;
	int		ping_socket;
	int		ping_ident;
	size_t	ping_errs;
	size_t	ping_data_len;
	float	ping_interval;
	int		ping_timeout;
	int		ping_ttl;

	int		ping_seq_num;
	size_t	ping_num_emit;
	size_t	ping_num_recv;
	size_t	ping_num_rept;
	int		ping_running;
	struct	sockaddr_in ping_addr;

	struct	timeval ping_start;
	struct	timeval	ping_time;

	int		ping_flag_v;	// verbose
	int		ping_flag_c;	// count
	int		ping_flag_q;	// quiet output

}	t_ping;

// Functions

void	ping_parse(int ac, char **av);
void	ping_send(void);
void	ping_socket_init(void);
void	ping_finish();

#endif