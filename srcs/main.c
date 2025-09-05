/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nponchon <nponchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 18:03:56 by nponchon          #+#    #+#             */
/*   Updated: 2025/09/05 18:21:09 by nponchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/ft_ping.h"

int	main(int ac, char **av) {
	if (ac < 2) {
		char *err = "Usage: ./ft_ping <IPv4> opt:<flags>\n";
		write(2, err, strlen(err));
		exit(1);
	}
	(void)av;
	return (0);
}