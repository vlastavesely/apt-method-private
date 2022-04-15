#include "hex.h"

static const char table[] = "0123456789abcdef";

std::string hexEncode(const unsigned char *bin, unsigned int len)
{
	std::string hex = "";
	unsigned int i;

	for (i = 0; i < len; i++) {
		hex.push_back(table[bin[i] >> 4]);
		hex.push_back(table[bin[i] & 15]);
	}

	return hex;
}
