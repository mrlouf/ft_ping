/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialise.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nponchon <nponchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 18:31:32 by nponchon          #+#    #+#             */
/*   Updated: 2025/09/05 18:38:26 by nponchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/ft_ping.h"

void	ping_initialise(char **av, t_ping *p)
{
	p = (t_ping*)malloc(sizeof(t_ping));
	if (p == NULL)
		exit(1);

	printf("initialise %s %p\n", av[1], &p);
}