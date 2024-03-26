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

long	ft_atol(const char *nptr)
{
	long	resp;
	long	signal;
	int		i;

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

int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	else
		return (0);
}