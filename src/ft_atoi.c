/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erramos <erramos@student.42.rio>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 15:09:27 by erramos           #+#    #+#             */
/*   Updated: 2024/03/25 15:09:30 by erramos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_atoi(const char *nptr)
{
	int	resp;
	int	signal;
	int	i;

	i = 0;
	resp = 0;
	signal = 1;
	while (nptr[i] == ' ' || (nptr[i] >= 9 && nptr[i] <= 13))
		i++;
	if (nptr[i] == '+' || nptr[i] == '-')
	{
		if (nptr[i] == '-')
			signal = signal *(-1);
		i++;
	}
	while (nptr[i] >= 48 && nptr[i] <= 57)
	{
		resp = resp * 10 + (nptr[i] - '0');
		i++;
	}
	return (resp * signal);
}