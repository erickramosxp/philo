/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erramos <erramos@student.42.rio>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 15:08:57 by erramos           #+#    #+#             */
/*   Updated: 2024/03/25 15:09:01 by erramos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/philo.h"
#include <stdio.h>

int	main(int argc, char **argv)
{
	struct timeval	tv;

	/*if ((argc - 1) < 4 || (argc - 1) > 5)
		return (0);*/
	gettimeofday(&tv, NULL);
	printf("%ld %ld", tv.tv_sec, tv.tv_usec);
	return (0);
}
