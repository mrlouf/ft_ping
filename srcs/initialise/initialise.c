/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialise.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nponchon <nponchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 18:31:32 by nponchon          #+#    #+#             */
/*   Updated: 2025/09/05 19:23:58 by nponchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/ft_ping.h"

void	ping_initialise(char **av, t_ping *p)
{
	av++;
	p->args = av;

	printf("initialise %s %p\n", p->args[0], p);
}