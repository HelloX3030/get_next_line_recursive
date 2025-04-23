#include "get_next_line.h"

// static int ft_strlen(char *str)
// {
// 	int i = 0;
// 	while (str[i])
// 		i++;
// 	return (i);
// }
// static void ft_putstr(char *str)
// {
// 	write(STDOUT_FILENO, str, ft_strlen(str));
// }
// static char *ft_strdup(char *str)
// {
// 	int len = ft_strlen(str);
// 	char *new = malloc(len + 1);
// 	for (int i = 0; i < len; i++)
// 		new[i] = str[i];
// 	new[len] = 0;
// 	return (new);
// }

char *get_result(int fd, const ssize_t carry_len, ssize_t pos, char *carry)
{
	char buffer[BUFFER_SIZE];
	char *result;
	ssize_t read_bytes = read(fd, buffer, BUFFER_SIZE);

	if (read_bytes == -1)
		return (NULL);
	else if (read_bytes == 0)
	{
		if (carry_len == 0 && pos == 0)
			return (NULL);
		else
		{
#ifdef PRINTS
			ft_putstr("End Reached!\n");
#endif
			result = malloc(carry_len + pos + 1);
			if (!result)
				return (NULL);
			for (ssize_t i = 0; i < carry_len; i++)
				result[i] = carry[i];
			result[carry_len + pos] = 0;
			carry[0] = 0;
			return (result);
		}
	}

#ifdef DEBUG
#ifdef PRINTS
	ft_putstr("Buffer: <");
	write(STDOUT_FILENO, buffer, BUFFER_SIZE);
	ft_putstr(">\n");
#endif
#endif

	// New Line Found?
	for (ssize_t n_pos = 0; n_pos < read_bytes; n_pos++)
	{
		if (buffer[n_pos] == '\n')
		{
			result = malloc(pos + carry_len + n_pos + 2);
			if (!result)
				return (NULL);

			// cpy in result
			for (ssize_t i = 0; i < carry_len; i++)
				result[i] = carry[i];
			for (ssize_t i = 0; i <= n_pos; i++)
				result[pos + carry_len + i] = buffer[i];
			result[pos + carry_len + n_pos + 1] = 0;

			// save rest in carry
			ssize_t i = 0;
			for (i = 0; n_pos + i + 1 < read_bytes; i++)
				carry[i] = buffer[n_pos + i + 1];
			carry[i] = 0;
			return (result);
		}
	}

	result = get_result(fd, carry_len, pos + read_bytes, carry);
	if (!result)
		return (NULL);
	for (ssize_t i = 0; i < read_bytes; i++)
		result[carry_len + pos + i] = buffer[i];
	return (result);
}

char *get_next_line(int fd)
{
	static char carry[BUFFER_SIZE + 1] = {0};

	if (BUFFER_SIZE <= 0)
		return (NULL);

	ssize_t carry_i;
	for (carry_i = 0; carry[carry_i]; carry_i++)
	{
		if (carry[carry_i] == '\n')
		{
			char *result = malloc(carry_i + 2);
			if (!result)
				return (NULL);
			for (ssize_t i = 0; i <= carry_i; i++)
				result[i] = carry[i];
			result[carry_i + 1] = 0;

			// Update Carry
			ssize_t i;
			for (i = 0; carry[carry_i + i + 1]; i++)
				carry[i] = carry[carry_i + i + 1];
			carry[i] = 0;
			return (result);
		}
	}

#ifdef PRINTS
	ft_putstr("Carry: <");
	ft_putstr(carry);
	ft_putstr(">\n");
#endif

	return (get_result(fd, carry_i, 0, carry));
}

#ifdef DEBUG
// Test Main
int main(void)
{
	int fd = open("test.txt", O_RDONLY);
	if (fd == -1)
	{
		ft_putstr("Error Opening File\n");
		return (1);
	}

	char *line = get_next_line(fd);
	if (!line)
		ft_putstr("No lines found\n");
	while (line)
	{
#ifdef PRINTS
		ft_putstr("line: <");
#endif
		ft_putstr(line);
#ifdef PRINTS
		ft_putstr(">\n");
#endif
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	return (0);
}
#endif
