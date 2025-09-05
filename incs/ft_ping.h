/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ping.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nponchon <nponchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 18:03:37 by nponchon          #+#    #+#             */
/*   Updated: 2025/09/05 18:37:21 by nponchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PING_H
# define FT_PING_H

// Libraries/Headers

# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <stdio.h>

// Typedefs

typedef struct s_ping
{
	char	**args;

}	t_ping;

// Functions

void	ping_parse(char **av);
void	ping_initialise(char **av, t_ping *p);

#endif