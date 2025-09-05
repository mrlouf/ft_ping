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

int	main(int ac, char **av) {
	if (ac < 2) {
		char *err = "Usage: ./ft_ping <IPv4> opt:<flags>\n";
		write(2, err, strlen(err));
		exit(1);
	}

	t_ping *p = (t_ping*)malloc(sizeof(t_ping));
	if (p == NULL) {
		exit(1);
	}

	ping_parse(av);
	ping_initialise(av, p);

	free(p);
	return (0);
}