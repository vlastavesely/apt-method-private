#include <fstream>
#include "random.h"

void randomise(char *buf, unsigned int len)
{
	std::ifstream in("/dev/urandom", std::ios::binary);

	if (!in.good())
		throw std::invalid_argument("Failed to read random data.");

	in.read(buf, len);
}
