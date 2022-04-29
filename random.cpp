#include <stdexcept>
#include <fcntl.h>
#include <unistd.h>
#include "random.h"

int randomise(unsigned char *buf, unsigned int len)
{
	int fd, n;

	fd = open("/dev/urandom", O_RDONLY);
	if (fd == -1)
		return -errno;

	n = read(fd, buf, len);
	if (n != (int) len) {
		int saved_errno = errno;
		close(fd);
		return -saved_errno;
	}

	return 0;
}
