#include <fstream>
#include "filter.h"

int encode_file(const std::string inFile, const std::string outFile,
		const unsigned char *key)
{
	std::ifstream in(inFile);
	std::ofstream out(outFile);
	char buf[16];
	int n;

	/* TODO FIXME - add real encryption */
	while (1) {
		n = in.readsome(buf, sizeof(buf));
		if (n == 0)
			break;

		out.write(buf, n);
	}

	return 0;
}

int decode_file(const std::string inFile, const std::string outFile,
		const unsigned char *key)
{
	std::ifstream in(inFile);
	std::ofstream out(outFile);
	char buf[16];
	int n;

	/* TODO FIXME - add real decryption */
	while (1) {
		n = in.readsome(buf, sizeof(buf));
		if (n == 0)
			break;

		out.write(buf, n);
	}

	return 0;
}
