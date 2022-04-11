#include <iostream>
#include "stanza.h"

int Stanza::parseHeader(const std::string &line)
{
	std::string key, val;
	size_t pos;

	pos = line.find(':');
	if (pos == std::string::npos)
		return -1;

	key = line.substr(0, pos);
	val = line.substr(pos + 1);

	fields[key] = val;

	return 0;
}

Stanza::Stanza(std::istream &in)
{
	std::string line;
	int ret;

	while (in.good()) {
		std::getline(in, line);

		if (line.length() == 0)
			break;

		ret = parseHeader(line);
		if (ret != 0) {
			throw std::invalid_argument("Invalid input.");
		}
	}
}
